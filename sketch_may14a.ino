#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// 定義腳位 (跟剛才一樣)
const int motorIN1 = 2;
const int motorIN2 = 3;

void setup() {
  Serial.begin(115200);
  Dabble.begin("Innovation_Balloon_Gen1"); // 這是你藍牙搜尋時會看到的名稱
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
}

void loop() {
  Dabble.processInput(); // 讀取手機訊號

  if (GamePad.isUpPressed()) {
    // 按住「上」時，馬達轉動
    analogWrite(motorIN1, 255);
    digitalWrite(motorIN2, LOW);
    Serial.println("Moving Forward...");
  } 
  else {
    // 沒按時，馬達停止
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
  }
}