# MotionSensor


# ESP8266 PIR Motion‑to‑Telegram Alarm

A lightweight IoT security node that detects motion with a **PIR sensor**, connects to Wi‑Fi via an **ESP8266** (e.g., NodeMCU or Wemos D1 mini) and pushes instant alerts to a Telegram chat.

---

## 📦 Bill of Materials
| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | ESP8266 development board | NodeMCU v3 / Wemos D1 mini |
| 1 | HC‑SR501 (or similar) PIR sensor | 3.3 V tolerant ✔︎ |
| 3 | Jumper wires | Female–female |
| 1 | Micro‑USB cable + 5 V supply | For flashing & power |

> Total hardware cost ≈ US $5–8.

---

## 🔌 Wiring
| PIR Pin | ESP8266 Pin | Note |
|---------|------------|------|
| VCC | 3V3 | 3.0–3.3 V only |
| OUT | D2 (GPIO4) | `pirPin` in code |
| GND | GND | Common ground |

PIR sensitivity & delay pots can be tuned with a screwdriver after deployment.

---

## 🛠️ Firmware Overview
```cpp
#include <ESP8266WiFi.h>
#include <WifiClientSecure.h>
#include "config.h"
// …
```
### Key Points
1. **`config.h`** holds Wi‑Fi credentials and Telegram tokens – keep it out of version control.
2. `sendTelegramMessage()` opens an HTTPS connection (`WiFiClientSecure`) to `api.telegram.org` and issues a simple GET request.
3. After a motion trigger is detected (`digitalRead(pirPin) == HIGH`), a message is sent and the loop sleeps 10 s to suppress spam.

---

## ⏩ Quick Start
1. **Clone** this repo.
2. Open `MotionToTelegram.ino` (or rename the snippet file) in **Arduino IDE** or **PlatformIO**.
3. Copy `config.sample.h` → `config.h` and fill in:
   ```c
   #define WIFI_SSID "YourNetwork"
   #define WIFI_PASSWORD "YourPassword"
   #define TELEGRAM_BOT_TOKEN "123456:ABC…"
   #define TELEGRAM_CHAT_ID "‑1001234567890"
   ```
4. Select target board **"NodeMCU 1.0 (ESP‑12E)"**, Flash frequency **80 MHz**, Upload speed **921600**.
5. Click **Upload**. Open **Serial Monitor @ 115200 baud** to watch connection logs.

> **Tip – Creating a Telegram bot**: Chat with **@BotFather**, generate a token, then send `/start` to your bot from the destination account to obtain the chat ID (use @myidbot or inspect the JSON reply).

---

## 🌐 Securing the Connection
- The sketch calls `client.setInsecure()` to skip TLS verification (easier, but MITM‑prone). For production, **store Telegram’s root CA** (`ISRG Root X1`) in flash and use `client.setTrustAnchors()`.
- Rate‑limit or CAPTCHA the bot if published publicly.

---

## 🔄 Extending the Project
| Idea | Hint |
|------|------|
| Home‑assistant integration | Publish MQTT instead of Telegram |
| Battery power | Use **deep sleep** & step‑up converter; wake on PIR interrupt |
| Multi‑chat broadcast | Iterate 💬 IDs array inside `sendTelegramMessage()` |
| Web dashboard | Add ESP8266WebServer + captive portal for Wi‑Fi provisioning |

---

## 📝 License
This project is released under the **MIT License** – see `LICENSE` for details.

---

### Author
Hooman Manesh  ·  2025 ©

