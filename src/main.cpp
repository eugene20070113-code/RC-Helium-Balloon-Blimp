#include <Arduino.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// ESP32-C3 SuperMini + DRV8833 + Dabble GamePad control
// Wiring:
// GPIO4 -> DRV8833 IN1
// GPIO5 -> DRV8833 IN2
// GPIO6 -> DRV8833 IN3
// GPIO7 -> DRV8833 IN4
// 3V3   -> DRV8833 EEP
// GND   -> DRV8833 GND

const int IN1 = 4;  // Left motor control 1
const int IN2 = 5;  // Left motor control 2
const int IN3 = 6;  // Right motor control 1
const int IN4 = 7;  // Right motor control 2

const int CRUISE_SPEED = 150;
const int TURN_SLOW_SPEED = 80;
const int TURN_FAST_SPEED = 170;

void setMotor(int pinA, int pinB, int speed);
void drive(int leftSpeed, int rightSpeed);
void stopMotors();
void handleGamePad();

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Dabble.begin("Innovation_Balloon_Gen1");
  Serial.println("ESP32-C3 + DRV8833 Dabble control ready");
}

void loop() {
  Dabble.processInput();
  handleGamePad();
  delay(20);
}

void handleGamePad() {
  bool up = GamePad.isUpPressed();
  bool down = GamePad.isDownPressed();
  bool left = GamePad.isLeftPressed();
  bool right = GamePad.isRightPressed();

  int leftSpeed = 0;
  int rightSpeed = 0;

  if (up) {
    leftSpeed = CRUISE_SPEED;
    rightSpeed = CRUISE_SPEED;
  } else if (down) {
    leftSpeed = -CRUISE_SPEED;
    rightSpeed = -CRUISE_SPEED;
  } else if (left) {
    leftSpeed = TURN_SLOW_SPEED;
    rightSpeed = TURN_FAST_SPEED;
  } else if (right) {
    leftSpeed = TURN_FAST_SPEED;
    rightSpeed = TURN_SLOW_SPEED;
  }

  if (up || down) {
    int direction = up ? 1 : -1;

    if (left) {
      leftSpeed = direction * TURN_SLOW_SPEED;
      rightSpeed = direction * TURN_FAST_SPEED;
    } else if (right) {
      leftSpeed = direction * TURN_FAST_SPEED;
      rightSpeed = direction * TURN_SLOW_SPEED;
    }
  }

  drive(leftSpeed, rightSpeed);
}

void setMotor(int pinA, int pinB, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    analogWrite(pinA, speed);
    analogWrite(pinB, 0);
  } else if (speed < 0) {
    analogWrite(pinA, 0);
    analogWrite(pinB, -speed);
  } else {
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  }
}

void drive(int leftSpeed, int rightSpeed) {
  setMotor(IN1, IN2, leftSpeed);
  setMotor(IN3, IN4, rightSpeed);
}

void stopMotors() {
  drive(0, 0);
}
