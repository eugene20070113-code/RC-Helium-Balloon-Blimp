# RC-Helium-Balloon-Blimp

ESP32-C3 SuperMini helium blimp motor-control test project.

## Hardware

- ESP32-C3 SuperMini
- DRV8833 motor driver module
- Two 720 coreless motors
- External motor battery

## Wiring

| ESP32-C3 SuperMini | DRV8833 |
| --- | --- |
| GPIO4 | IN1 |
| GPIO5 | IN2 |
| GPIO6 | IN3 |
| GPIO7 | IN4 |
| 3V3 | EEP |
| GND | GND |

| DRV8833 | Connection |
| --- | --- |
| VCC | Motor battery positive |
| GND | Motor battery negative |
| OUT1, OUT2 | Left motor |
| OUT3, OUT4 | Right motor |

The ESP32-C3 GND, DRV8833 GND, and battery negative must share the same ground.

## Current Test Program

The current firmware tests the motors in this order:

1. Left motor forward for 0.5 seconds
2. Stop for 2 seconds
3. Left motor backward for 0.5 seconds
4. Stop for 2 seconds
5. Right motor forward for 0.5 seconds
6. Stop for 2 seconds
7. Right motor backward for 0.5 seconds
8. Stop for 3 seconds

Do not install propellers during the first test.

## PlatformIO

This project can be opened in VS Code with PlatformIO.

- Environment: `esp32-c3-supermini`
- Upload port: `COM10`
- Serial monitor speed: `115200`

## Arduino IDE

The same logic is also available in:

`arduino/sketch_may14a/sketch_may14a.ino`
