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

1. Forward: both motors run forward with the same thrust for 1.5 seconds
2. Stop for 2 seconds
3. Backward: both motors run backward with the same thrust for 1.5 seconds
4. Stop for 2 seconds
5. Turn left: left motor runs weaker, right motor runs stronger for 1.5 seconds
6. Stop for 2 seconds
7. Turn right: left motor runs stronger, right motor runs weaker for 1.5 seconds
8. Stop for 3 seconds

The test uses PWM speed values from `0` to `255`:

- Cruise speed: `150`
- Slow turn-side speed: `80`
- Fast turn-side speed: `170`

Do not install propellers during the first test.

## Progress Reports

- [2026-05-22 Differential Motor Test](docs/progress-2026-05-22.md)

## PlatformIO

This project can be opened in VS Code with PlatformIO.

- Environment: `esp32-c3-supermini`
- Upload port: `COM10`
- Serial monitor speed: `115200`

## Arduino IDE

The same logic is also available in:

`arduino/sketch_may14a/sketch_may14a.ino`
