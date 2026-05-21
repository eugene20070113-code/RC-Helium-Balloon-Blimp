# RC-Helium-Balloon-Blimp

ESP32-C3 SuperMini helium blimp motor-control prototype.  
ESP32-C3 SuperMini 氦氣飛船馬達控制原型。

## Project Overview / 專案簡介

This project uses an ESP32-C3 SuperMini as the main controller and a DRV8833 motor driver to control two 720 coreless motors. The two motors are mounted on the left and right sides of the blimp, allowing forward, backward, and turning movement through differential thrust.

本專案使用 ESP32-C3 SuperMini 作為主控板，搭配 DRV8833 馬達驅動模組控制兩顆 720 空心杯馬達。兩顆馬達分別安裝在飛船左右兩側，透過左右推力差實現前進、後退與轉向。

## Hardware / 使用硬體

- ESP32-C3 SuperMini
- DRV8833 motor driver module / DRV8833 馬達驅動模組
- Two 720 coreless motors / 兩顆 720 空心杯馬達
- 104 ceramic capacitors / 104 陶瓷電容
- 100uF electrolytic capacitor / 100uF 電解電容
- External motor battery / 外接馬達電池

## Wiring / 接線

### ESP32-C3 to DRV8833 / ESP32-C3 接 DRV8833

| ESP32-C3 SuperMini | DRV8833 | Description / 說明 |
| --- | --- | --- |
| GPIO4 | IN1 | Left motor control 1 / 左馬達控制 1 |
| GPIO5 | IN2 | Left motor control 2 / 左馬達控制 2 |
| GPIO6 | IN3 | Right motor control 1 / 右馬達控制 1 |
| GPIO7 | IN4 | Right motor control 2 / 右馬達控制 2 |
| 3V3 | EEP | Enable DRV8833 / 啟用 DRV8833 |
| GND | GND | Shared ground / 共地 |

### Motor and Power / 馬達與電源

| DRV8833 | Connection / 接法 |
| --- | --- |
| VCC | Motor battery positive / 馬達電池正極 |
| GND | Motor battery negative / 馬達電池負極 |
| OUT1, OUT2 | Left motor / 左馬達 |
| OUT3, OUT4 | Right motor / 右馬達 |

The ESP32-C3 GND, DRV8833 GND, and motor battery negative must share the same ground.  
ESP32-C3 的 GND、DRV8833 的 GND、馬達電池負極必須接在一起，也就是共地。

## Capacitors / 電容配置

| Component / 元件 | Placement / 位置 | Purpose / 作用 |
| --- | --- | --- |
| 104 ceramic capacitor / 104 陶瓷電容 | Across left motor terminals / 並聯在左馬達兩端 | Reduce motor noise / 降低馬達雜訊 |
| 104 ceramic capacitor / 104 陶瓷電容 | Across right motor terminals / 並聯在右馬達兩端 | Reduce motor noise / 降低馬達雜訊 |
| 100uF electrolytic capacitor / 100uF 電解電容 | Between DRV8833 VCC and GND / 接在 DRV8833 VCC 與 GND 之間 | Stabilize motor power / 穩定馬達電源 |

The 104 ceramic capacitors do not have polarity. The 100uF electrolytic capacitor has polarity: positive to DRV8833 VCC, negative to DRV8833 GND.  
104 陶瓷電容沒有正負極。100uF 電解電容有正負極，正極接 DRV8833 VCC，負極接 DRV8833 GND。

## Current Test Program / 目前測試程式

The current firmware tests the motors in this order:  
目前程式會依照以下順序測試馬達：

1. Forward: both motors run forward at speed `150` for 1.5 seconds.  
   前進：左右馬達都以速度 `150` 正轉 1.5 秒。
2. Stop for 2 seconds.  
   停止 2 秒。
3. Backward: both motors run backward at speed `150` for 1.5 seconds.  
   後退：左右馬達都以速度 `150` 反轉 1.5 秒。
4. Stop for 2 seconds.  
   停止 2 秒。
5. Turn left: left motor runs at speed `80`, right motor runs at speed `170` for 1.5 seconds.  
   左轉：左馬達速度 `80`，右馬達速度 `170`，持續 1.5 秒。
6. Stop for 2 seconds.  
   停止 2 秒。
7. Turn right: left motor runs at speed `170`, right motor runs at speed `80` for 1.5 seconds.  
   右轉：左馬達速度 `170`，右馬達速度 `80`，持續 1.5 秒。
8. Stop for 3 seconds, then repeat.  
   停止 3 秒後重複。

## PWM Speed Settings / PWM 速度設定

The test uses PWM speed values from `0` to `255`.  
目前使用 PWM 控制馬達速度，數值範圍是 `0` 到 `255`。

```cpp
const int CRUISE_SPEED = 150;
const int TURN_SLOW_SPEED = 80;
const int TURN_FAST_SPEED = 170;
```

## Safety Notes / 安全注意事項

- Do not install propellers during early bench tests.  
  初期桌上測試時不要安裝螺旋槳。
- Disconnect the motor battery after confirming the test sequence.  
  確認測試動作後，請拔掉馬達電池。
- Do not leave the test program running continuously for a long time.  
  不要讓測試程式長時間連續運轉。
- If the driver, motor, or battery becomes hot, disconnect power immediately.  
  如果驅動板、馬達或電池明顯發熱，請立刻斷電。

## Progress Reports / 進度報告

- [2026-05-22 Differential Motor Test / 雙馬達差速測試](docs/progress-2026-05-22.md)

## PlatformIO / VS Code

This project can be opened in VS Code with PlatformIO.  
本專案可以使用 VS Code 搭配 PlatformIO 開啟。

- Environment / 環境: `esp32-c3-supermini`
- Upload port / 燒錄埠: `COM10`
- Serial monitor speed / 序列監控鮑率: `115200`

## Arduino IDE

The same logic is also available in the Arduino IDE sketch:  
相同邏輯也有 Arduino IDE 版本：

`arduino/sketch_may14a/sketch_may14a.ino`
