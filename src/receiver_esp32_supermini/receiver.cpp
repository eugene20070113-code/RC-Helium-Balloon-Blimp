#include <esp_now.h>
#include <WiFi.h>


// ==========================================
// 📍 硬體腳位定義 (ESP32-C3 SuperMini <-> DRV8833)
// ==========================================
const int IN1 = 4;  // 左馬達 (前進)
const int IN2 = 5;  // 左馬達 (後退)
const int IN3 = 6;  // 右馬達 (前進)
const int IN4 = 7;  // 右馬達 (後退)


// ==========================================
// ☁️ 動力參數 - 克服新馬達嗡嗡叫升級版
// ==========================================
// 🚀 關鍵修改 1：最低啟動推力直接從 95 起跳，跳過馬達會嗡嗡叫的低電壓死區！
const int START_POWER = 95;      
const int MAX_POWER = 150;        // 🚀 直線最高推力
const int JOYSTICK_DEADZONE = 120;// 🛡️ 搖桿飄移防護罩
const float STEERING_SENSITIVITY = 0.5; // 🧭 轉向靈敏度砍半，過彎更平滑


// ==========================================
// 🛡️ 軟啟動專用變數 (太空船漂浮感加速)
// ==========================================
int targetLeft = 0;
int targetRight = 0;
int currentLeft = 0;
int currentRight = 0;
const int RAMP_STEP = 10;         // ☁️ 雲朵提速
// 🚀 關鍵修改 2：起步瞬間直接給予的「踢力」，確保一開始就有 95 的力道讓馬達轉起來
const int DEAD_ZONE_JUMP = 95;    


unsigned long lastRecvTime = 0;


// ==========================================
// 📡 ESP-NOW 封包結構 + 驗證機制
// ==========================================
typedef struct struct_message {
  int x;           // 左右推力 (-255 到 255)
  int y;           // 前後推力 (-255 到 255)
  uint8_t checksum;// 🔒 簡單校驗和，確保資料完整無竄改
} struct_message;


struct_message myData;


// ==========================================
// 🔒 校驗和計算函數 (CRC8 簡易版)
// ==========================================
uint8_t calculateChecksum(int x, int y) {
  uint8_t sum = 0;
  sum += (x & 0xFF);           // X 的低位元組
  sum += ((x >> 8) & 0xFF);    // X 的高位元組
  sum += (y & 0xFF);           // Y 的低位元組
  sum += ((y >> 8) & 0xFF);    // Y 的高位元組
  return sum;
}


// ==========================================
// 📡 ESP-NOW 接收回調函數
// ==========================================
void OnDataRecv(const esp_now_recv_info *esp_now_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastRecvTime = millis();


  // 🔒 驗證校驗和 (防止訊號竄改)
  uint8_t expectedChecksum = calculateChecksum(myData.x, myData.y);
  if (myData.checksum != expectedChecksum) {
    Serial.println("⚠️ 警告：接收到不完整或被竄改的訊號，略過本次指令");
    return;  // 拒絕執行被竄改的命令
  }


  int throttle = myData.x;
  // 🛡️ 轉向數值直接套用靈敏度打折，並加上「負號 (-) 」來反轉左右方向！
  int steering = -(myData.y * STEERING_SENSITIVITY);


  // 過濾遙控器雜訊 (死區防護)
  if (abs(throttle) < JOYSTICK_DEADZONE) throttle = 0;
  if (abs(myData.y) < JOYSTICK_DEADZONE) steering = 0; // 注意看原始 Y 數值


  // 計算混控動力
  int leftPower = throttle + steering;
  int rightPower = throttle - steering;


  leftPower = constrain(leftPower, -255, 255);
  rightPower = constrain(rightPower, -255, 255);


  // 🚀 動力映射：確保只要有推動，推力絕對在 95 (START_POWER) 到 150 (MAX_POWER) 之間！
  if (leftPower > 0) targetLeft = map(leftPower, 0, 255, START_POWER, MAX_POWER);
  else if (leftPower < 0) targetLeft = map(leftPower, -255, 0, -MAX_POWER, -START_POWER);
  else targetLeft = 0;


  if (rightPower > 0) targetRight = map(rightPower, 0, 255, START_POWER, MAX_POWER);
  else if (rightPower < 0) targetRight = map(rightPower, -255, 0, -MAX_POWER, -START_POWER);
  else targetRight = 0;
}


void setup() {
  Serial.begin(115200);


  // 設定馬達控制腳位為輸出模式
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  // 啟動時強制鎖死馬達策安全
  stopMotors();


  // 初始化 ESP-NOW
  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW 初始化失敗！");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);


  Serial.println("==========================================");
  Serial.println("🚁 飛船 ESP32-C3 SuperMini");
  Serial.println("ESP-NOW 接收端已啟動 (含校驗和驗證)");
  Serial.println("==========================================");
  Serial.println("⏳ 等待遙控器訊號...");
}


void loop() {
  // 🛡️ 終極斷訊保護：超過 0.5 秒沒收到遙控器訊號，啟動安全煞車
  if (millis() - lastRecvTime > 500) {
    targetLeft = 0; 
    targetRight = 0;
    currentLeft = 0; 
    currentRight = 0;
    stopMotors();
  } else {
    // 正常連線時，執行平滑加速邏輯
    drive(targetLeft, targetRight);
  }
 
  // 控制迴圈更新頻率
  delay(20);
}


// ==========================================
// ⚙️ 底層驅動與平滑防護罩
// ==========================================
// 將正負數值的速度，轉換為雙腳位的 PWM 輸出 (純淨無雜訊版)
void setMotor(int pinA, int pinB, int speed) {
  speed = constrain(speed, -255, 255);


  if (speed == 0) {
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  } else if (speed > 0) {
    analogWrite(pinB, 0);
    analogWrite(pinA, speed);
  } else {
    analogWrite(pinA, 0);
    analogWrite(pinB, -speed);
  }
}


// 平滑加速 (Soft-Start) 控制引擎
void drive(int tLeft, int tRight) {
  // 跨越死區機制：給馬達足夠的初始電壓踢力，避免嗡嗡叫
  if (tLeft != 0 && currentLeft == 0) currentLeft = (tLeft > 0) ? DEAD_ZONE_JUMP : -DEAD_ZONE_JUMP;
  if (tRight != 0 && currentRight == 0) currentRight = (tRight > 0) ? DEAD_ZONE_JUMP : -DEAD_ZONE_JUMP;


  // 緩步追蹤左馬達目標
  if (currentLeft < tLeft) currentLeft += RAMP_STEP;
  else if (currentLeft > tLeft) currentLeft -= RAMP_STEP;


  // 緩步追蹤右馬達目標
  if (currentRight < tRight) currentRight += RAMP_STEP;
  else if (currentRight > tRight) currentRight -= RAMP_STEP;


  // 防震盪貼齊
  if (abs(currentLeft - tLeft) <= RAMP_STEP) currentLeft = tLeft;
  if (abs(currentRight - tRight) <= RAMP_STEP) currentRight = tRight;


  // 輸出平滑後的真實速度到馬達
  setMotor(IN1, IN2, currentLeft);
  setMotor(IN3, IN4, currentRight);
}


// 緊急煞車與歸零
void stopMotors() {
  currentLeft = 0;
  currentRight = 0;
  setMotor(IN1, IN2, 0);
  setMotor(IN3, IN4, 0);
}
