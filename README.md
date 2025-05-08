# ESP8266 PIR Motion‑to‑Telegram Alarm

A compact IoT security system that detects motion with a PIR sensor, connects via ESP8266, and triggers a Telegram alert while logging the event to an AWS RDS MySQL database using an AWS Lambda function.

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

## 🛠 Firmware Architecture

### Arduino (.ino) Sketch

- Connects ESP8266 to Wi-Fi
- Monitors the PIR sensor (`digitalRead(pirPin)`)
- If motion is detected, sends a **HTTPS POST** request to an **AWS Lambda endpoint** using `WiFiClientSecure`
- Sleeps for 10 seconds to reduce duplicate triggers

### AWS Lambda Function (`lambda_function.py`)

- Parses incoming POST body
- Sends a **Telegram message** using bot token and chat ID
- Logs each event into an **RDS MySQL** table `motion_events`

> Environment variables in Lambda store Telegram credentials and database connection info securely.

---

## ⏩ Quick Start

### 1. Clone and Configure
- Clone this repo.
- Open `wirelessnetworkmain.ino` (or `MotionToTelegram.ino`) in **Arduino IDE** or **PlatformIO**.
- Copy `config.sample.h` → `config.h` and edit:
  ```cpp
  #define WIFI_SSID "YourNetwork"
  #define WIFI_PASSWORD "YourPassword"
  #define LAMBDA_ENDPOINT "your-api-id.lambda-url.amazonaws.com"
  ```

### 2. Flash the ESP8266
- Board: **NodeMCU 1.0 (ESP-12E)**
- Flash frequency: **80 MHz**
- Upload speed: **921600**
- Serial baud rate: **115200**
- Click **Upload**, then open **Serial Monitor** to verify connection logs.

### 3. Setup AWS Lambda + RDS
- Deploy `lambda_function.py` to an AWS Lambda function.
- Set the following environment variables:
  - `TELEGRAM_BOT_TOKEN`
  - `TELEGRAM_CHAT_ID`
  - `DB_HOST`
  - `DB_USER`
  - `DB_PASS`
  - `DB_NAME`
- Make sure:
  - Lambda has internet access (e.g., public subnet or NAT gateway)
  - RDS is accessible from the Lambda environment (via VPC or public endpoint)

### ✅ Telegram Bot Setup Tip
- Chat with **@BotFather** to generate a bot token.
- Send `/start` to your bot from the destination Telegram account.
- Use **@myidbot** or inspect Telegram API JSON response to find the `chat_id`.

---


## 🔄 Extending the Project
| Idea | Hint |
|------|------|
| Home‑assistant integration | Publish MQTT instead of Telegram |
| Battery power | Use **deep sleep** & step‑up converter; wake on PIR interrupt |
| Multi‑chat broadcast | Iterate 💬 IDs array inside `sendTelegramMessage()` |
| Web dashboard | Add ESP8266WebServer + captive portal for Wi‑Fi provisioning |

---

## 🔐 Security Notes

- Uses `client.setInsecure()` in ESP sketch (not safe for production). For enhanced security:
  - Store **Telegram's ISRG Root X1** certificate on device
  - Use `client.setTrustAnchors()`
- Telegram and DB credentials are kept in Lambda environment variables

---


## 📝 License
This project is released under the **MIT License** – see `LICENSE` for details.

---

### Authors
Hooman Manesh  ·  2025 ©  
Jeffrey DeOcampo  ·  2025 ©  
Jaecob Adajar  ·  2025 ©  
Erasto Jr Damian Astudillo  ·  2025 ©

