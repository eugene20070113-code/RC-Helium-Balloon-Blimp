// ESP32-C3 SuperMini + DRV8833 motor test
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

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Serial.println("ESP32-C3 + DRV8833 motor test ready");
  delay(2000);
}

void loop() {
  Serial.println("Left motor forward");
  leftForward();
  delay(500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Left motor backward");
  leftBackward();
  delay(500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Right motor forward");
  rightForward();
  delay(500);

  Serial.println("Stop");
  stopMotors();
  delay(2000);

  Serial.println("Right motor backward");
  rightBackward();
  delay(500);

  Serial.println("Stop");
  stopMotors();
  delay(3000);
}

void leftForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void leftBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void rightForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
