# RC-Helium-Balloon-Blimp 專案進度總結

**最後更新**: 2026年8月29日

## 專案概述

RC直升機氣球/飛艇電機控制原型。使用ESP32-C3 SuperMini控制DRV8833馬達驅動器，以駕駛兩個720無芯馬達。

## 核心功能

- ✅ 雙馬達差分推力控制
- ✅ PWM速度控制（0-255）
- ✅ 前進、後退、轉向控制
- ✅ 馬達噪音減弱電路（去耦電容）

## 項目進度歷史

### 第1階段：初始設置 (提交: `df6b0e1`)
**狀態**: ✅ 完成
- 專案初始化
- 基礎硬體連線配置
- 文檔框架建立

**提交信息**: `feat: 完成單馬達藍牙遙控測試`

---

### 第2階段：單馬達藍牙遙控測試 (提交: `452175a`)
**狀態**: ✅ 完成
- 初始馬達測試專案建立
- 單馬達驅動測試
- 藍牙遙控基礎實現

**提交信息**: `Initial motor test project`

---

### 第3階段：差分馬達測試 (提交: `95198ef`)
**狀態**: ✅ 完成
- 升級為雙馬達控制
- 差分推力控制實現
- PWM速度控制參數設置

**測試序列**:
1. 前進：左右馬達速度150，1.5秒
2. 停止：2秒
3. 後退：左右馬達速度150，1.5秒
4. 停止：2秒
5. 左轉：左馬達速度80，右馬達速度170，1.5秒
6. 停止：2秒
7. 右轉：左馬達速度170，右馬達速度80，1.5秒
8. 停止：3秒

**測試結果**:
- ✅ 編譯成功
- ✅ 上傳成功
- ✅ 硬體檢測正常
- ✅ 馬達正常運作

**提交信息**: `Add differential motor test progress report`

---

### 第4階段：文檔改進 (提交: `366c047` - `23c984c`)
**狀態**: ✅ 完成
- 進度報告添加日期標記
- README.md雙語化（英文/中文）
- 文檔結構分離優化

**相關提交**:
- `366c047`: Add date to progress report
- `0e93481`: Make README bilingual
- `23c984c`: Separate English and Chinese documentation sections

---

## 硬體配置

### 主控制器
- **型號**: ESP32-C3 SuperMini
- **連接**: GPIO4-7 連接馬達驅動器

### 馬達驅動器
- **型號**: DRV8833 馬達驅動模塊
- **馬達**: 2x 720無芯馬達（左右側）

### 連線對應表

| ESP32-C3 | DRV8833 | 功能 |
|----------|---------|------|
| GPIO4 | IN1 | 左馬達控制1 |
| GPIO5 | IN2 | 左馬達控制2 |
| GPIO6 | IN3 | 右馬達控制1 |
| GPIO7 | IN4 | 右馬達控制2 |
| 3V3 | EEP | 啟用DRV8833 |
| GND | GND | 共同接地 |

### 保護元件

| 元件 | 位置 | 功能 |
|------|------|------|
| 104陶瓷電容 | 左馬達端子間 | 降低馬達電噪音 |
| 104陶瓷電容 | 右馬達端子間 | 降低馬達電噪音 |
| 100μF電解電容 | DRV8833 VCC/GND間 | 穩定馬達電源 |

## 當前韌體設置

### PWM速度參數
```cpp
const int CRUISE_SPEED = 150;      // 巡航速度
const int TURN_SLOW_SPEED = 80;    // 轉向慢速
const int TURN_FAST_SPEED = 170;   // 轉向快速
```

### PWM範圍
- 最小: 0
- 最大: 255

## 專案檔案結構

```
RC-Helium-Balloon-Blimp/
├── README.md                           # 項目說明（英文/中文）
├── platformio.ini                      # PlatformIO配置
├── sketch_may14a.ino                   # 主要草圖
├── src/
│   └── main.cpp                        # 主程式源碼
├── arduino/
│   └── sketch_may14a/
│       └── sketch_may14a.ino           # Arduino版本
└── docs/
    ├── progress-2026-05-22.md          # 階段進度報告
    └── progress-summary-2026.md        # 本文件
```

## GitHub提交日誌

| 提交ID | 信息 | 狀態 |
|--------|------|------|
| `23c984c` | Separate English and Chinese documentation sections | ✅ main |
| `0e93481` | Make README bilingual | ✅ |
| `366c047` | Add date to progress report | ✅ |
| `95198ef` | Add differential motor test progress report | ✅ |
| `452175a` | Initial motor test project | ✅ |
| `df6b0e1` | feat: 完成單馬達藍牙遙控測試 | ✅ |

**目前分支**: main
**遠程狀態**: 最新（已同步）

## 已驗證功能

- ✅ ESP32-C3 可成功與DRV8833通訊
- ✅ GPIO控制引腳正常工作
- ✅ PWM速度控制有效
- ✅ 左馬達獨立控制
- ✅ 右馬達獨立控制
- ✅ 差分推力轉向功能
- ✅ 馬達保護電路有效

## 下一步計畫

待定

---

**工作區**: `g:\飛船專題_2026\vs code\RC-Helium-Balloon-Blimp`
