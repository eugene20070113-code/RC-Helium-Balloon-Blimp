// ESP32-C3 SuperMini + DRV8833 differential motor test
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

// PWM duty range is 0-255. Start conservatively for bench testing.
const int CRUISE_SPEED = 150;
const int TURN_SLOW_SPEED = 80;
const int TURN_FAST_SPEED = 170;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Serial.println("ESP32-C3 + DRV8833 differential test ready");
  delay(2000);
}

void loop() {
  Serial.println("Forward: both motors same thrust");
  forward();
  delay(1500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Backward: both motors reverse");
  backward();
  delay(1500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Turn left: left motor weaker, right motor stronger");
  turnLeft();
  delay(1500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Turn right: left motor stronger, right motor weaker");
  turnRight();
  delay(1500);

  Serial.println("Stop");
  stopMotors();
  delay(3000);
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

void forward() {
  drive(CRUISE_SPEED, CRUISE_SPEED);
}

void backward() {
  drive(-CRUISE_SPEED, -CRUISE_SPEED);
}

void turnLeft() {
  drive(TURN_SLOW_SPEED, TURN_FAST_SPEED);
}

void turnRight() {
  drive(TURN_FAST_SPEED, TURN_SLOW_SPEED);
}

void stopMotors() {
  drive(0, 0);
}
