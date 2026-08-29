#include <esp_now.h>
#include <WiFi.h>


// ==========================================
// 📍 遙控器硬體腳位 (傳統大塊 ESP32 安全腳位)
// ==========================================
const int PIN_X = 32;  // 香菇頭 VRx
const int PIN_Y = 33;  // 香菇頭 VRy


// ==========================================
// 🎈 綁定飛船大腦的身分證字號 (MAC Address)
// ==========================================
// ⚠️ 重要：請使用你的飛船 MAC 地址替換下面的數值！
// 
// 📝 如何查詢飛船的 MAC 地址：
// 1. 先燒錄飛船端程式到 ESP32-C3 SuperMini
// 2. 打開序列監控 (Serial Monitor)，選擇 115200 波特率
// 3. 會看到像這樣的訊息：
//    🚁 飛船 ESP32-C3 SuperMini
//    📡 MAC Address: 10:00:3B:CC:3E:6C
// 4. 複製 MAC 地址，替換下面的 {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}
//
// ❌ 舊範例 (請勿使用)：{0x10, 0x00, 0x3B, 0xCC, 0x3E, 0x6C}
// ✅ 新範例 (根據你的飛船替換)：{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
// 
uint8_t broadcastAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};  // 👈 替換成你的飛船 MAC


// ==========================================
// 📡 訊號結構 + 安全驗證機制
// ==========================================
typedef struct struct_message {
  int x;           // 左右推力 (-255 到 255)
  int y;           // 前後推力 (-255 到 255)
  uint8_t checksum;// 🔒 校驗和，防止訊號竄改
} struct_message;


struct_message myData;
esp_now_peer_info_t peerInfo;


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


void setup() {
  Serial.begin(115200);

  delay(1000);  // 等待序列埠就緒
  
  Serial.println("==========================================");
  Serial.println("📡 遙控器 (ESP32 + 香菇搖桿)");
  Serial.println("==========================================");


  // 檢查 MAC 地址是否被設置
  if (broadcastAddress[0] == 0xFF && 
      broadcastAddress[1] == 0xFF && 
      broadcastAddress[2] == 0xFF && 
      broadcastAddress[3] == 0xFF && 
      broadcastAddress[4] == 0xFF && 
      broadcastAddress[5] == 0xFF) {
    Serial.println("❌ 錯誤：MAC 地址未設置！");
    Serial.println("📝 請按照程式碼註釋，使用飛船的 MAC 地址替換 broadcastAddress");
    Serial.println("⏸️ 程式已暫停");
    while(1) delay(1000);  // 無限等待，迫使用戶設置 MAC
  }

  if (broadcastAddress[0] == 0xXX) {
    Serial.println("❌ 錯誤：MAC 地址仍為預設值 (0xXX...)！");
    Serial.println("📝 請替換成你的飛船 MAC 地址");
    Serial.println("⏸️ 程式已暫停");
    while(1) delay(1000);  // 無限等待
  }

  Serial.print("📍 目標飛船 MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", broadcastAddress[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println("\n");


  // 1. 開啟 Wi-Fi 模式 (ESP-NOW 必須使用 Station 模式)
  WiFi.mode(WIFI_MODE_STA);


  // 2. 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW 初始化失敗！");
    return;
  }


  // 3. 註冊飛船為通訊對象
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;  // 🔓 注意：訊號未加密。如需安全性，請啟用加密
 
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("❌ 無法連接到飛船節點");
    return;
  }

  Serial.println("✅ 遙控器已準備就緒！");
  Serial.println("🎮 搖動香菇頭開始控制飛船...\n");
}


void loop() {
  // 1. 讀取香菇頭的類比數值 (大 ESP32 的 ADC 範圍是 0 ~ 4095)
  int rawX = analogRead(PIN_X);
  int rawY = analogRead(PIN_Y);


  // 2. 將 0~4095 轉換為 -255 到 255 的推力數值 (2048是搖桿置中)
  int mappedX = map(rawX, 0, 4095, -255, 255);
  int mappedY = map(rawY, 0, 4095, -255, 255);


  // 3. 🛡️ 搖桿置中防飄移死區 (如果搖桿放開時沒有完全回正 0，強制歸零)
  if (abs(mappedX) < 20) mappedX = 0;
  if (abs(mappedY) < 20) mappedY = 0;


  // 4. 裝箱打包 + 🔒 計算校驗和
  myData.x = mappedX;
  myData.y = mappedY;
  myData.checksum = calculateChecksum(mappedX, mappedY);


  // 5. 發射訊號給飛船！
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));


  // 控制發射頻率 (大約每秒 50 次，極低延遲)
  delay(20);
}
