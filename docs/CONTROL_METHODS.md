# 遙控控制方案詳解

本專案支援兩種無線遙控方案。本文件詳細說明每種方案的硬體配置、代碼部署和使用方式。

---

## 方案 1：手機 Dabble App 遙控 (Bluetooth)

### 特點
- ✅ 無需額外硬體
- ✅ 直接用手機控制
- ✅ 適合快速測試和演示
- ✅ 支援多種手機遊戲搖桿模擬

### 硬體需求
- 飛船：ESP32-C3 SuperMini
- 手機：iOS 或 Android
- 連接方式：Bluetooth 無線

### 軟體安裝
1. 手機下載 **Dabble** app
   - [iOS App Store](https://apps.apple.com/app/dabble-iot-for-arduino/id1461351821)
   - [Google Play Store](https://play.google.com/store/apps/details?id=io.dabble)

2. 飛船端代碼：`src/main.cpp` (原始測試版本)

### 使用流程
1. 將飛船代碼燒錄到 ESP32-C3 SuperMini
2. 打開手機 Dabble app
3. 點擊「Connect」搜尋藍牙設備
4. 選擇飛船的藍牙名稱並配對
5. 打開 Dabble 的「遊戲搖桿」功能
6. 用搖桿控制飛船的前進、後退、轉向

### 優點 & 缺點

| 方面 | 評價 |
|------|------|
| 成本 | 💰 最便宜（無需額外硬體） |
| 便利性 | ⭐⭐⭐⭐ 手機即拿即用 |
| 操控感 | ⭐⭐⭐ 可接受，但手機觸控不如物理搖桿 |
| 延遲 | ⚠️ 略高（Bluetooth 延遲約 50-100ms） |
| 有效距離 | 📍 ~10-15 公尺 |
| 適用場景 | 🏠 室內或近距離測試 |

---

## 方案 2：ESP32 + 香菇搖桿遙控 (ESP-NOW)

### 特點
- ✅ 專用硬體遙控器，操控更精確
- ✅ 香菇搖桿提供類比控制
- ✅ ESP-NOW 低延遲、高可靠性
- ✅ 更適合長時間飛行與精密操控

### 硬體需求

#### 飛船端 (接收器)
- **ESP32-C3 SuperMini**
- **DRV8833 馬達驅動模組**
- **2x 720 空心杯馬達**
- **Orbz 氣球**
- **保護元件**：2x 104 陶瓷電容 + 1x 100μF 電解電容

#### 遙控器端 (發射器)
- **ESP32** (大型版或 DevKit)
  - 需要模擬輸入腳位（ADC）
- **香菇搖桿模組**
  - VRx 連接 GPIO32
  - VRy 連接 GPIO33
  - +5V 和 GND 連接

### 硬體連接圖

#### 飛船端接線
```
ESP32-C3 SuperMini  ←→  DRV8833
GPIO4 (IN1)         ←→  IN1
GPIO5 (IN2)         ←→  IN2
GPIO6 (IN3)         ←→  IN3
GPIO7 (IN4)         ←→  IN4
3V3                 ←→  EEP
GND                 ←→  GND

DRV8833             ←→  馬達
OUT1, OUT2          ←→  左馬達
OUT3, OUT4          ←→  右馬達
VCC                 ←→  馬達電池 (+)
GND                 ←→  馬達電池 (-)
```

#### 遙控器端接線
```
香菇搖桿            ←→  ESP32
VRx                 ←→  GPIO32
VRy                 ←→  GPIO33
+5V                 ←→  5V
GND                 ←→  GND
```

### 代碼部署

#### 步驟 1：查詢飛船的 MAC 地址

1. 編譯並上傳飛船代碼：
   ```
   src/receiver_esp32_supermini/receiver.cpp
   ```

2. 打開 Serial Monitor（波特率：115200）

3. 查看輸出信息，找到飛船的 MAC 地址：
   ```
   🚁 飛船 ESP32-C3 SuperMini
   📡 MAC Address: 10:00:3B:CC:3E:6C
   ```

4. **記下這個 MAC 地址**（例如：10:00:3B:CC:3E:6C）

#### 步驟 2：配置遙控器代碼

1. 打開遙控器代碼：
   ```
   src/transmitter_esp32/transmitter.cpp
   ```

2. 找到這一行：
   ```cpp
   uint8_t broadcastAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
   ```

3. 根據步驟 1 的 MAC 地址替換：
   ```cpp
   // 範例：MAC 地址 10:00:3B:CC:3E:6C
   uint8_t broadcastAddress[] = {0x10, 0x00, 0x3B, 0xCC, 0x3E, 0x6C};
   ```

4. 保存並編譯

#### 步驟 3：燒錄代碼

1. 將飛船代碼燒錄到 **ESP32-C3 SuperMini**
2. 將遙控器代碼燒錄到 **ESP32 (遙控器)**
3. 分別打開兩個 Serial Monitor 查看狀態

### 使用流程

1. **啟動飛船**
   - 電源打開，飛船進入待機狀態
   - Serial Monitor 顯示：`⏳ 等待遙控器訊號...`

2. **啟動遙控器**
   - 打開遙控器電源
   - Serial Monitor 顯示：`✅ 遙控器已準備就緒！`
   - 等待與飛船配對

3. **開始控制**
   - 搖動香菇頭控制飛船
   - X 軸：左右轉向
   - Y 軸：前進/後退

4. **停止**
   - 將搖桿放回中心位置（自動歸零）
   - 如果超過 0.5 秒無訊號，飛船自動停止 (安全保護)

### 訊號協議

#### 訊號結構
```cpp
typedef struct struct_message {
  int x;           // 左右推力 (-255 到 255)
  int y;           // 前後推力 (-255 到 255)
  uint8_t checksum;// 校驗和 (防止訊號竄改)
} struct_message;
```

#### 校驗和機制
- 每個訊號都包含 CRC8 校驗和
- 接收端會驗證資料完整性
- 如果校驗失敗，訊號會被拒絕
- 這確保惡意或破損的訊號無法控制飛船

#### 更新頻率
- 遙控器每 20ms 發送一次訊號（~50 Hz）
- 飛船每 20ms 更新一次馬達控制
- 總延遲：< 50ms

### 優點 & 缺點

| 方面 | 評價 |
|------|------|
| 成本 | 💰💰 需購買額外硬體 (~USD 10-20) |
| 便利性 | ⭐⭐⭐⭐⭐ 專用搖桿，操控最直觀 |
| 操控感 | ⭐⭐⭐⭐⭐ 類比搖桿，精確度高 |
| 延遲 | ⚡ 極低 (< 50ms) |
| 有效距離 | 📍 ~100+ 公尺（取決於環境） |
| 適用場景 | 🌳 戶外飛行、長時間操控、精密控制 |

---

## 安全特性

### 所有方案共有

1. **斷訊保護**
   - 如果 0.5 秒內未收到訊號，飛船自動停止
   - 防止因連線中斷導致飛船失控

2. **死區防護**
   - 搖桿±20 以內的微小漂移被忽略
   - 防止手指離開搖桿時的無意輸入

3. **加速平滑**
   - 馬達速度逐步加速而非突然跳躍
   - 增加控制穩定性

4. **功率限制** (方案 2 特有)
   - 馬達速度被限制在 95-150 之間
   - 避免馬達在低電壓下發生故障

### 方案 2 額外安全機制

5. **校驗和驗證**
   - 每個訊號都有 CRC8 校驗和
   - 防止訊號竄改或損毀

6. **MAC 地址綁定**
   - 遙控器只能與指定的飛船通訊
   - 他人無法隨意控制你的飛船

---

## 控制邏輯說明

### 馬達混控系統

飛船採用「差分推力控制」，通過左右馬達速度差實現轉向：

```
leftMotor = throttle + steering
rightMotor = throttle - steering
```

- **throttle (X軸)**: 前進/後退
- **steering (Y軸)**: 左轉/右轉

### 轉向靈敏度

- 預設轉向靈敏度係數：`0.5`
- 這意味著轉向輸入會被削減一半
- 目的：確保轉向不會過度靈敏

如果你覺得轉向太靈或太遲鈍，可以修改這個值：
```cpp
const float STEERING_SENSITIVITY = 0.5;  // 改成 0.3 (更柔和) 或 0.7 (更靈敏)
```

---

## 故障排除

### 方案 1：Dabble 遙控

| 問題 | 解決方案 |
|------|---------|
| 手機無法找到藍牙設備 | 檢查 ESP32 藍牙是否已啟用；嘗試重新啟動藍牙 |
| 控制延遲很高 | 靠近飛船；檢查是否有干擾；嘗試切換藍牙頻道 |
| 飛船不動 | 檢查馬達電池是否充電；確認所有接線無誤 |

### 方案 2：ESP32 + 搖桿遙控

| 問題 | 解決方案 |
|------|---------|
| 遙控器找不到飛船 | 確認 MAC 地址是否正確；檢查飛船是否已上電 |
| Serial Monitor 顯示 "❌ MAC 地址未設置" | 用正確的飛船 MAC 替換 `0xXX` |
| 控制反應遲鈍 | 檢查搖桿是否損壞；嘗試重新校準ADC |
| 飛船突然停止 | 可能是訊號喪失 (斷訊保護)；檢查遙控器電源 |

---

## 推薦使用情景

### 選擇方案 1 (Dabble):
- 🏠 在家進行快速測試
- 👥 向朋友演示
- 📊 進行數據記錄和分析 (Dabble 有內建圖表)
- 🔧 調試馬達參數

### 選擇方案 2 (ESP32 + 搖桿):
- 🌳 戶外飛行測試
- ⏱️ 長時間操控 (搖桿不會因握持而疲勞)
- 🎯 精密飛行操控
- 🏆 競速或特技飛行

---

## 進階設置

### 修改馬達功率範圍

編輯 `receiver.cpp`：
```cpp
const int START_POWER = 95;       // 最低啟動推力
const int MAX_POWER = 150;        // 最高推力
```

- 降低 `MAX_POWER` → 飛船速度較慢但更穩定
- 提高 `MAX_POWER` → 飛船速度更快但消耗更多功率

### 調整轉向靈敏度

編輯 `receiver.cpp`：
```cpp
const float STEERING_SENSITIVITY = 0.5;
```

- 改成 `0.3` → 轉向變遲鈍 (適合初學者)
- 改成 `0.7` → 轉向變靈敏 (適合高手)

### 修改斷訊保護時間

編輯 `receiver.cpp`：
```cpp
if (millis() - lastRecvTime > 500) {  // 500ms
```

- 改成 `1000` → 允許 1 秒的訊號喪失時間
- 改成 `200` → 更靈敏的保護 (建議不要太短)

---

## 相關資源

- [ESP-NOW 官方文檔](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
- [Dabble 官方網站](https://thestempedia.com/dabble/)
- [ESP32 ADC 特性](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)

---

**最後更新**: 2026年8月29日
