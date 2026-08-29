# RC-Helium-Balloon-Blimp

ESP32-C3 SuperMini based RC helium balloon blimp prototype. This project uses a DRV8833 motor driver to control two 720 coreless motors, and uses the Dabble mobile app GamePad module as the Bluetooth remote controller.

The current stage focuses on making the blimp move smoothly by using differential thrust. The left and right motors can run forward, backward, and at different speeds, allowing straight movement and wide-radius turning.

## Current Status

| Item | Current Setting |
| --- | --- |
| Main controller | ESP32-C3 SuperMini |
| Motor driver | DRV8833 |
| Motors | Two 720 coreless motors |
| Remote-control app | Dabble |
| Dabble module | GamePad |
| Bluetooth device name | `Innovation_Balloon` |
| Development environment | VS Code + PlatformIO |
| PlatformIO environment | `esp32-c3-supermini` |
| Main firmware file | `src/main.cpp` |
| Arduino sketch copies | `sketch_may14a.ino`, `arduino/sketch_may14a/sketch_may14a.ino` |

## Main Design Idea

The blimp uses two side motors instead of a steering rudder. Movement is controlled by changing the thrust of the left and right motors:

- Forward: both motors push forward at the same speed.
- Backward: both motors reverse at the same speed.
- Left turn: right motor runs faster than the left motor.
- Right turn: left motor runs faster than the right motor.
- Arc movement: forward or backward movement is combined with left/right turning.

This turning method is more suitable for a helium blimp because it avoids sudden in-place spinning. The current firmware uses wide-radius turns so the motion is smoother and easier to control.

## Hardware List

#### Control Electronics
- ESP32-C3 SuperMini
- DRV8833 motor driver module
- External motor battery

#### Motors and Propulsion
- Two 720 coreless motors
- Orbz balloon (structural body)
- Helium (lifting gas)

#### Protection Components
- 104 ceramic capacitors (2x)
- 100uF electrolytic capacitor

#### Remote Control Options
- **Option 1**: Smartphone via Dabble app (Bluetooth)
- **Option 2**: ESP32 + Mushroom joystick transmitter

### Wiring

### 1. Power Center and Main Switch

The battery first goes through the slide switch, then enters the breadboard power rail. This makes the switch the main power control for the whole system.

| Wire / Pin | Destination | Purpose |
| --- | --- | --- |
| Battery black wire / negative | Breadboard blue GND rail | Common ground |
| Battery red wire / positive | Center pin of the 3-pin slide switch | Power input to switch |
| Left or right output pin of slide switch | Breadboard red VCC rail | Switched power output |

After this step, the breadboard red rail is the main `VCC`, and the blue rail is the main `GND`.

### 2. ESP32-C3 SuperMini Power

| ESP32-C3 SuperMini | Destination | Note |
| --- | --- | --- |
| 5V | Breadboard red VCC rail | Use this pin for the current wiring |
| GND | Breadboard blue GND rail | Shared ground |

Important: do not connect the main power rail to the ESP32-C3 `3V3` pin. The current wiring plan powers the board through `5V`.

### 3. DRV8833 Power and Enable

| DRV8833 Pin | Destination | Purpose |
| --- | --- | --- |
| VCC | Breadboard red VCC rail | Motor driver power |
| GND | Breadboard blue GND rail | Shared ground |
| EEP | Breadboard red VCC rail | Enable / wake line |

The `EEP` pin must be connected to VCC. If `EEP` is not connected, the DRV8833 may stay disabled and the motors will not run even if the signal wires are correct.

### 4. ESP32-C3 to DRV8833 Signal Wiring

| ESP32-C3 GPIO | DRV8833 Input | Firmware Constant | Motor Side |
| --- | --- | --- | --- |
| GPIO4 | IN1 | `IN1` | Left motor forward side |
| GPIO5 | IN2 | `IN2` | Left motor backward side |
| GPIO6 | IN3 | `IN3` | Right motor forward side |
| GPIO7 | IN4 | `IN4` | Right motor backward side |

The firmware uses `analogWrite()` on these pins to control motor speed through PWM.

### 5. Motor Output Wiring

| Motor | DRV8833 Output |
| --- | --- |
| Left 720 coreless motor | OUT1 and OUT2 |
| Right 720 coreless motor | OUT3 and OUT4 |

If one motor spins in the wrong direction, swap the two wires of that motor on the DRV8833 output. If both motors move backward when pressing forward, the direction can also be corrected in firmware.

## Capacitors and Protection

| Component | Placement | Polarity | Purpose |
| --- | --- | --- | --- |
| 100uF electrolytic capacitor | Near DRV8833, between VCC and GND rails | Long leg to VCC, short leg / stripe side to GND | Helps stabilize motor power |
| 104 ceramic capacitor | Across the left motor terminals | No polarity | Reduces motor electrical noise |
| 104 ceramic capacitor | Across the right motor terminals | No polarity | Reduces motor electrical noise |

The 720 coreless motors can create electrical noise and sudden current changes. The capacitors help reduce noise and make the power supply more stable, especially when the motors start moving.

### Remote Control

The blimp supports two wireless control methods:

#### Method 1: Smartphone (Dabble App)
- Uses Bluetooth wireless connection
- Control via mobile device running Dabble app
- Suitable for quick testing and demonstrations

#### Method 2: ESP32 + Mushroom Joystick
- Uses a separate ESP32 transmitter with mushroom joystick
- Direct wireless control between two ESP32 boards
- More ergonomic for extended flying sessions
- Better range and reliability for field operations

### Current Test Program

### Bluetooth Control

The firmware uses Dabble ESP32 with custom settings enabled:

```cpp
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
```

The Bluetooth name shown in the Dabble app is:

```cpp
Dabble.begin("Innovation_Balloon");
```

### Speed Settings

```cpp
const int CRUISE_SPEED = 255;
const int TURN_FAST_SPEED = 255;
const int TURN_SLOW_SPEED = 120;
const int RAMP_SPEED = 15;
```

| Constant | Meaning |
| --- | --- |
| `CRUISE_SPEED` | Speed used for straight forward/backward movement |
| `TURN_FAST_SPEED` | Speed of the outside motor during turning |
| `TURN_SLOW_SPEED` | Speed of the inside motor during turning |
| `RAMP_SPEED` | Maximum speed change per loop cycle |

PWM speed values range from `0` to `255`.

### Soft-Start / Smooth Acceleration

The firmware does not immediately jump from `0` to full speed. Instead, it stores the current motor speed in:

```cpp
int currentLeftSpeed = 0;
int currentRightSpeed = 0;
```

The `drive()` function gradually moves the current speed toward the target speed by `RAMP_SPEED`. This reduces sudden motor startup current and helps prevent ESP32-C3 resets caused by voltage drops.

### Control Behavior

| Dabble GamePad Input | Left Motor | Right Motor | Blimp Movement |
| --- | --- | --- | --- |
| Up | Forward fast | Forward fast | Straight forward |
| Down | Backward fast | Backward fast | Straight backward |
| Left | Forward slow | Forward fast | Wide left turn |
| Right | Forward fast | Forward slow | Wide right turn |
| Up + Left | Forward slow | Forward fast | Forward left arc |
| Up + Right | Forward fast | Forward slow | Forward right arc |
| Down + Left | Backward slow | Backward fast | Backward left arc |
| Down + Right | Backward fast | Backward slow | Backward right arc |
| No button | Ramps down to 0 | Ramps down to 0 | Smooth stop |

## PlatformIO / VS Code

This project is configured for VS Code with PlatformIO.

| Setting | Value |
| --- | --- |
| PlatformIO environment | `esp32-c3-supermini` |
| Board | `esp32-c3-devkitm-1` |
| Framework | Arduino |
| Upload port | `COM10` |
| Monitor port | `COM10` |
| Monitor speed | `115200` |

Build command:

```powershell
C:\Users\01034\.platformio\penv\Scripts\pio.exe run
```

The latest build result was successful.

## Testing Checklist

1. Confirm the battery negative line is connected to the breadboard GND rail.
2. Confirm the battery positive line goes through the slide switch before reaching the VCC rail.
3. Confirm ESP32-C3 uses `5V` and `GND`.
4. Confirm DRV8833 `VCC`, `GND`, and `EEP` are connected.
5. Confirm GPIO4, GPIO5, GPIO6, and GPIO7 match IN1, IN2, IN3, and IN4.
6. Confirm the 100uF capacitor polarity is correct.
7. Confirm each 104 capacitor is soldered across the motor terminals.
8. Upload the firmware.
9. Open Dabble and connect to `Innovation_Balloon`.
10. Test without propellers first.
11. Press each direction button briefly and confirm the motor direction.

## Troubleshooting

| Problem | Possible Cause | Fix |
| --- | --- | --- |
| Motors do not move | DRV8833 `EEP` not connected | Connect `EEP` to VCC |
| ESP32-C3 resets when motors start | Motor startup current causes voltage drop | Check capacitor, battery strength, and lower speed/ramp settings |
| One motor spins backward | Motor output wires reversed | Swap the two wires of that motor |
| Left/right control feels reversed | Motor side or output direction is reversed | Swap motor wires or adjust firmware |
| Dabble cannot find the board | Bluetooth not started or wrong board powered | Check ESP32-C3 power and look for `Innovation_Balloon` |
| Upload fails | Wrong COM port | Check Device Manager or PlatformIO port setting |

## Progress Reports

- [2026-05-29 Dabble Remote Control and Final Wiring](docs/progress-2026-05-29.md)
- [2026-05-26 Current Progress Report](docs/progress-2026-05-26.md)
- [2026-05-22 Differential Motor Test](docs/progress-2026-05-22.md)

## Safety Notes

This project can be opened in VS Code with PlatformIO.

- Environment: `esp32-c3-supermini`
- Upload port: `COM10`
- Serial monitor speed: `115200`

### Arduino IDE

The same logic is also available in the Arduino IDE sketch:

`arduino/sketch_may14a/sketch_may14a.ino`

## 中文

### 專案簡介

本專案是使用 ESP32-C3 SuperMini 製作的氦氣飛船馬達控制原型。系統透過 DRV8833 馬達驅動模組控制兩顆 720 空心杯馬達。兩顆馬達分別安裝在飛船左右兩側，藉由左右推力差實現前進、後退與轉向。

### 使用硬體

#### 控制電子元件
- ESP32-C3 SuperMini
- DRV8833 馬達驅動模組
- 外接馬達電池

#### 馬達與推進系統
- 兩顆 720 空心杯馬達
- Orbz 氣球（結構體）
- 氦氣（升力氣體）

#### 保護元件
- 104 陶瓷電容 (2個)
- 100uF 電解電容

#### 遙控方式
- **方案 1**：手機 Dabble app 遙控（藍牙無線）
- **方案 2**：ESP32 + 蘑菇搖桿遙控

### 接線

#### ESP32-C3 接 DRV8833

| ESP32-C3 SuperMini | DRV8833 | 說明 |
| --- | --- | --- |
| GPIO4 | IN1 | 左馬達控制 1 |
| GPIO5 | IN2 | 左馬達控制 2 |
| GPIO6 | IN3 | 右馬達控制 1 |
| GPIO7 | IN4 | 右馬達控制 2 |
| 3V3 | EEP | 啟用 DRV8833 |
| GND | GND | 共地 |

#### 馬達與電源

| DRV8833 | 接法 |
| --- | --- |
| VCC | 馬達電池正極 |
| GND | 馬達電池負極 |
| OUT1, OUT2 | 左馬達 |
| OUT3, OUT4 | 右馬達 |

ESP32-C3 的 GND、DRV8833 的 GND、馬達電池負極必須接在一起，也就是共地。

### 電容配置

| 元件 | 位置 | 作用 |
| --- | --- | --- |
| 104 陶瓷電容 | 並聯在左馬達兩端 | 降低馬達雜訊 |
| 104 陶瓷電容 | 並聯在右馬達兩端 | 降低馬達雜訊 |
| 100uF 電解電容 | 接在 DRV8833 VCC 與 GND 之間 | 穩定馬達電源 |

104 陶瓷電容沒有正負極。100uF 電解電容有正負極，正極接 DRV8833 VCC，負極接 DRV8833 GND。

### 遙控控制

飛船支援兩種無線控制方式：

#### 方案 1：手機 Dabble App
- 使用藍牙無線連接
- 透過執行 Dabble app 的手機進行控制
- 適合快速測試與演示

#### 方案 2：ESP32 + 蘑菇搖桿
- 使用另一塊 ESP32 搭配蘑菇搖桿作為發射器
- 兩塊 ESP32 之間的直接無線控制
- 操作更符合人體工學，適合長時間飛行
- 可提供更好的控制距離與可靠性

### 目前測試程式

目前程式會依照以下順序測試馬達：

1. 前進：左右馬達都以速度 `150` 正轉 1.5 秒。
2. 停止 2 秒。
3. 後退：左右馬達都以速度 `150` 反轉 1.5 秒。
4. 停止 2 秒。
5. 左轉：左馬達速度 `80`，右馬達速度 `170`，持續 1.5 秒。
6. 停止 2 秒。
7. 右轉：左馬達速度 `170`，右馬達速度 `80`，持續 1.5 秒。
8. 停止 3 秒後重複。

### PWM 速度設定

目前使用 PWM 控制馬達速度，數值範圍是 `0` 到 `255`。

```cpp
const int CRUISE_SPEED = 150;
const int TURN_SLOW_SPEED = 80;
const int TURN_FAST_SPEED = 170;
```

### 安全注意事項

- 初期桌上測試時不要安裝螺旋槳。
- 確認測試動作後，請拔掉馬達電池。
- 不要讓測試程式長時間連續運轉。
- 如果驅動板、馬達或電池明顯發熱，請立刻斷電。

### 進度報告

- [2026-05-22 雙馬達差速測試](docs/progress-2026-05-22.md)

### PlatformIO / VS Code

本專案可以使用 VS Code 搭配 PlatformIO 開啟。

- 環境：`esp32-c3-supermini`
- 燒錄埠：`COM10`
- 序列監控鮑率：`115200`

### Arduino IDE

相同邏輯也有 Arduino IDE 版本：

`arduino/sketch_may14a/sketch_may14a.ino`
