# RC-Helium-Balloon-Blimp

## English

### Project Overview

This project is an ESP32-C3 SuperMini based helium blimp motor-control prototype. It uses a DRV8833 motor driver to control two 720 coreless motors. The two motors are placed on the left and right sides of the blimp, allowing forward movement, backward movement, and turning through differential thrust.

### Hardware

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

#### ESP32-C3 to DRV8833

| ESP32-C3 SuperMini | DRV8833 | Description |
| --- | --- | --- |
| GPIO4 | IN1 | Left motor control 1 |
| GPIO5 | IN2 | Left motor control 2 |
| GPIO6 | IN3 | Right motor control 1 |
| GPIO7 | IN4 | Right motor control 2 |
| 3V3 | EEP | Enable DRV8833 |
| GND | GND | Shared ground |

#### Motor and Power

| DRV8833 | Connection |
| --- | --- |
| VCC | Motor battery positive |
| GND | Motor battery negative |
| OUT1, OUT2 | Left motor |
| OUT3, OUT4 | Right motor |

The ESP32-C3 GND, DRV8833 GND, and motor battery negative must share the same ground.

### Capacitors

| Component | Placement | Purpose |
| --- | --- | --- |
| 104 ceramic capacitor | Across the left motor terminals | Reduce motor noise |
| 104 ceramic capacitor | Across the right motor terminals | Reduce motor noise |
| 100uF electrolytic capacitor | Between DRV8833 VCC and GND | Stabilize motor power |

The 104 ceramic capacitors do not have polarity. The 100uF electrolytic capacitor has polarity: positive to DRV8833 VCC and negative to DRV8833 GND.

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

The current firmware tests the motors in this order:

1. Forward: both motors run forward at speed `150` for 1.5 seconds.
2. Stop for 2 seconds.
3. Backward: both motors run backward at speed `150` for 1.5 seconds.
4. Stop for 2 seconds.
5. Turn left: left motor runs at speed `80`, right motor runs at speed `170` for 1.5 seconds.
6. Stop for 2 seconds.
7. Turn right: left motor runs at speed `170`, right motor runs at speed `80` for 1.5 seconds.
8. Stop for 3 seconds, then repeat.

### PWM Speed Settings

The test uses PWM speed values from `0` to `255`.

```cpp
const int CRUISE_SPEED = 150;
const int TURN_SLOW_SPEED = 80;
const int TURN_FAST_SPEED = 170;
```

### Safety Notes

- Do not install propellers during early bench tests.
- Disconnect the motor battery after confirming the test sequence.
- Do not leave the test program running continuously for a long time.
- If the driver, motor, or battery becomes hot, disconnect power immediately.

### Progress Reports

- [2026-05-22 Differential Motor Test](docs/progress-2026-05-22.md)

### PlatformIO / VS Code

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
