#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// ESP32-C3 SuperMini + DRV8833 + Dabble GamePad control
// Power wiring:
// Battery - -> breadboard GND rail
// Battery + -> center pin of 3-pin slide switch
// Switch output -> breadboard VCC rail
// ESP32-C3 5V -> VCC rail, GND -> GND rail
// DRV8833 VCC -> VCC rail, GND -> GND rail, EEP -> VCC rail
// Signal wiring:
// GPIO4 -> DRV8833 IN1
// GPIO5 -> DRV8833 IN2
// GPIO6 -> DRV8833 IN3
// GPIO7 -> DRV8833 IN4

const int IN1 = 4;  // Left motor forward
const int IN2 = 5;  // Left motor backward
const int IN3 = 6;  // Right motor forward
const int IN4 = 7;  // Right motor backward

const int CRUISE_SPEED = 255;
const int TURN_FAST_SPEED = 255;
const int TURN_SLOW_SPEED = 120;
const int RAMP_SPEED = 15;

int currentLeftSpeed = 0;
int currentRightSpeed = 0;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Dabble.begin("Innovation_Balloon");
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

  if (up || down) {
    int direction = up ? 1 : -1;

    if (left) {
      leftSpeed = direction * TURN_SLOW_SPEED;
      rightSpeed = direction * TURN_FAST_SPEED;
    } else if (right) {
      leftSpeed = direction * TURN_FAST_SPEED;
      rightSpeed = direction * TURN_SLOW_SPEED;
    } else {
      leftSpeed = direction * CRUISE_SPEED;
      rightSpeed = direction * CRUISE_SPEED;
    }
  } else if (left) {
    leftSpeed = TURN_SLOW_SPEED;
    rightSpeed = TURN_FAST_SPEED;
  } else if (right) {
    leftSpeed = TURN_FAST_SPEED;
    rightSpeed = TURN_SLOW_SPEED;
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

void drive(int targetLeft, int targetRight) {
  if (currentLeftSpeed < targetLeft) {
    currentLeftSpeed += RAMP_SPEED;
  } else if (currentLeftSpeed > targetLeft) {
    currentLeftSpeed -= RAMP_SPEED;
  }

  if (currentRightSpeed < targetRight) {
    currentRightSpeed += RAMP_SPEED;
  } else if (currentRightSpeed > targetRight) {
    currentRightSpeed -= RAMP_SPEED;
  }

  if (abs(currentLeftSpeed - targetLeft) <= RAMP_SPEED) {
    currentLeftSpeed = targetLeft;
  }
  if (abs(currentRightSpeed - targetRight) <= RAMP_SPEED) {
    currentRightSpeed = targetRight;
  }

  setMotor(IN1, IN2, currentLeftSpeed);
  setMotor(IN3, IN4, currentRightSpeed);
}

void stopMotors() {
  currentLeftSpeed = 0;
  currentRightSpeed = 0;

  setMotor(IN1, IN2, 0);
  setMotor(IN3, IN4, 0);
}
