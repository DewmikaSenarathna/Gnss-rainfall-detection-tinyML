# NimbusAI - Combined Rainfall Prediction System Setup Guide

## System Architecture

```
┌─────────────────────────────────┐
│       STM32 Microcontroller     │
│  (STM32_RainfallMonitor.ino)    │
├─────────────────────────────────┤
│ • DHT11: Temperature & Humidity │
│ • Rain Sensor: Analog Input     │
│ • GPS Module: GNSS SNR Values   │
│ • OLED Display: 128x32          │
│ • ML Model: Rain Prediction     │
└────────────┬────────────────────┘
             │ Serial2 (TX/RX)
             │ 115200 baud
             ▼
┌─────────────────────────────────┐
│    ESP8266 WiFi Module          │
│ (ESP8266_WiFiFirebase.ino)      │
├─────────────────────────────────┤
│ • WiFi Connection               │
│ • Firebase Realtime Database    │
│ • Web Server (/data endpoint)   │
│ • Mobile App Sync               │
└────────────┬────────────────────┘
             │ HTTPS
             ▼
      ┌─────────────────┐
      │  Firebase RTDB  │
      │   RainData      │
      └────────┬────────┘
               │
               ▼
        ┌──────────────────┐
        │ NimbusAI Mobile  │
        │      App         │
        └──────────────────┘
```

## Hardware Wiring

### STM32 Configuration

| Component | Pin | Notes |
|-----------|-----|-------|
| DHT11 | Pin 2 | Temperature & Humidity |
| Rain Sensor | A0 | Analog Input (0-1023) |
| GPS Module | Serial1 | TX/RX at 9600 baud |
| OLED Display | I2C | SDA/SCL at 0x3C address |
| ESP8266 TX | PA3 (RX) | Receive data from ESP8266 |
| ESP8266 RX | PA2 (TX) | Send data to ESP8266 |
| **Serial Monitor** | USART1 | Debug output at 9600 baud |

### ESP8266 Configuration

| Component | Pin | Notes |
|-----------|-----|-------|
| STM32 TX | RX (GPIO3) | Data from STM32 |
| STM32 RX | TX (GPIO1) | Send to STM32 |
| WiFi Antenna | - | Internal or external |
| **Serial Monitor** | USB-TTL | Debug at 115200 baud |

## Hardware Serial Communication Protocol

### STM32 → ESP8266 (Serial2)
**Format:** `TEMP,HUMIDITY,MAXSNR,RAINVALUE,RAINLEVEL\n`

**Example Data:**
```
28.50,65.20,42,512,1
```

**Fields:**
- `TEMP`: Temperature in °C (float, 2 decimals)
- `HUMIDITY`: Humidity in % (float, 2 decimals)
- `MAXSNR`: Maximum satellite SNR value (int, filtered > 60)
- `RAINVALUE`: Raw rain sensor value (int, 0-1023)
- `RAINLEVEL`: Predicted rain level (int: 0=No Rain, 1=Low Rain, 2=High Rain)

### Error Handling
- STM32 sends data every 10 seconds
- If no data received for 5 seconds, ESP8266 logs warning
- Invalid format messages are logged and skipped

## Firebase Data Structure

### Path: `/RainData.json`

```json
{
  "created_at": "2026-03-22 14:30",
  "temperatur": 28.50,
  "humidity": 65.20,
  "maxSNR": 42,
  "rainValue": 512,
  "rainLevel": 1
}
```

**Note:** Firebase key is `temperatur` (not `temperature`) to maintain compatibility with your existing mobile app schema.

## Configuration & Setup Steps

### 1. STM32 Setup (STM32_RainfallMonitor.ino)

**Libraries Required:**
- Adafruit_GFX
- Adafruit_SSD1306 (OLED)
- DHT (DHT sensor)

**Installation:**
```
Arduino IDE → Sketch → Include Library → Manage Libraries
Search and install:
- "Adafruit GFX Library"
- "Adafruit SSD1306"
- "DHT sensor library"
```

**Compile & Upload:**
1. Open `STM32_RainfallMonitor.ino` in Arduino IDE
2. Select Board: "STM32 Nucleo-64" (or your STM32 board)
3. Select Port: COM port of STM32
4. Click Upload

**Verify:**
- Open Serial Monitor at 9600 baud
- You should see debug messages and sensor readings
- OLED display shows: Temp, Humidity, MaxSNR, Rain Level

### 2. ESP8266 Setup (ESP8266_WiFiFirebase.ino)

**Libraries Required:**
- ESP8266WiFi (built-in)
- ESP8266HTTPClient (built-in)
- ESP8266WebServer (built-in)

**Configuration:**
Edit these lines in the code:
```cpp
const char* ssid = "Galaxy";              // Your WiFi SSID
const char* password = "dondew0027";      // Your WiFi password
const char* firebaseHost = "rainfallprediction-945cd-default-rtdb...";  // Your Firebase URL
```

**Get Firebase Host:**
1. Go to Firebase Console
2. Select your project
3. Realtime Database → Settings
4. Copy the "Database URL" (without https:// and trailing /)

**Compile & Upload:**
1. Open `ESP8266_WiFiFirebase.ino` in Arduino IDE
2. Select Board: "NodeMCU 1.0 (ESP8266-12E)" (or your ESP8266 board)
3. Select Port: COM port of ESP8266
4. Click Upload

**Verify:**
- Open Serial Monitor at 115200 baud
- You should see WiFi connection status
- Data received messages from STM32
- Firebase send responses

### 3. Serial Connection Verification

**Check Data Flow:**

**On STM32 Serial Monitor (9600 baud):**
```
========== STM32 Rainfall Monitor Starting ==========
GPS Serial initialized
WiFi Serial (to ESP8266) initialized
DHT11 initialized
Rain sensor initialized
OLED initialized

Temp: 28.50°C | Hum: 65.20% | MaxSNR: 42 | RainValue: 512 | RainLevel: 1
Sent to ESP8266: 28.50,65.20,42,512,1
```

**On ESP8266 Serial Monitor (115200 baud):**
```
========== ESP8266 WiFi/Firebase Module Starting ==========
Connecting to WiFi: Galaxy
WiFi Connected!
IP Address: 192.168.x.x

Data parsed successfully from STM32
========== SENDING TO FIREBASE ==========
URL: https://rainfallprediction-945cd...
Payload: {"created_at":"2026-03-22 14:30","temperatur":28.50,...}
Response Code: 200
```

## Features & Functionality

### STM32 Features
✅ Real-time temperature & humidity monitoring (DHT11)
✅ Satellite SNR data collection via GPS (GNSS)
✅ Automatic SNR filtering (removes values > 60)
✅ ML-based rain intensity prediction
✅ OLED display (128x32) with real-time updates
✅ Serial communication with ESP8266 at 115200 baud
✅ Data sent every 10 seconds

### ESP8266 Features
✅ WiFi connectivity with auto-reconnect
✅ Real-time Firebase uploads
✅ Timestamp synchronization via NTP
✅ Local REST API endpoint (`/data`)
✅ Mobile app integration ready
✅ CORS headers for cross-origin requests
✅ Comprehensive error logging

### Data Processing
✅ SNR filtering: Only values 1-60 accepted
✅ ML model for rain classification (3 levels)
✅ Automatic data buffering
✅ Continuous monitoring loop

## ML Model Details

**Rain Level Classification:**

| rainLevel | Description | Condition |
|-----------|-------------|-----------|
| 0 | **No Rain** | z0 score highest |
| 1 | **Low Rain** | z1 score highest |
| 2 | **High Rain** | z2 score highest |

**Model Coefficients (Training-based):**
```
z0 = (11.03*T) + (-16.76*H) + (4.15*SNR) + (-8.37*Rain) + 1.33  → No Rain
z1 = (0.43*T) + (3.78*H) + (0.77*SNR) + (-3.01*Rain) + 0.13    → Low Rain
z2 = (-11.46*T) + (12.98*H) + (-4.91*SNR) + (11.37*Rain) - 1.46 → High Rain
```

**Note:** These coefficients are pre-trained. To retrain with your Firebase data, export data from Firebase and use Python with scikit-learn.

## Mobile App Integration

### API Endpoints

**Get Current Data:**
```
GET http://<ESP8266_IP>/data
Content-Type: application/json

Response:
{
  "created_at": "2026-03-22 14:30",
  "temperatur": 28.50,
  "humidity": 65.20,
  "maxSNR": 42,
  "rainValue": 512,
  "rainLevel": 1
}
```

**Firebase Real-time Listener:**
```
Listen to: /RainData
Auto-updates every 10 seconds with latest data
```

## Troubleshooting

### Issue: No data appears on OLED
- **Check:** DHT11 wiring and I2C OLED address (0x3C)
- **Solution:** Run I2C scanner to verify address

### Issue: ESP8266 shows "WiFi not connected"
- **Check:** SSID and password configuration
- **Solution:** Verify WiFi credentials in code

### Issue: Firebase shows null values
- **Check:** Serial communication between STM32 and ESP8266
- **Solution:** Monitor both serial ports simultaneously

### Issue: GPS SNR values not updating
- **Check:** GPS module connection on Serial1
- **Solution:** Verify GPS module output format ($GPGSV sentences)

### Issue: Temperature readings incorrect
- **Check:** DHT11 sensor connection
- **Solution:** Try DHT22 for better accuracy

## Continuous Operation & Model Retraining

### Automatic Data Collection
- System collects data every 10 seconds
- Firebase stores all historical data
- Data includes: timestamp, temperature, humidity, SNR, rain values

### Export Data for Model Retraining
1. Go to Firebase Console → Realtime Database
2. Export JSON → Download
3. Use Python script to retrain model:

```python
import json
import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler

# Load Firebase data
with open('firebase_export.json') as f:
    data = json.load(f)

# Prepare dataset
temperatures = [d['temperatur'] for d in data.values()]
humidities = [d['humidity'] for d in data.values()]
snrs = [d['maxSNR'] for d in data.values()]
rains = [d['rainValue'] for d in data.values()]
levels = [d['rainLevel'] for d in data.values()]

# Train new model
X = StandardScaler().fit_transform(
    list(zip(temperatures, humidities, snrs, rains))
)
clf = LogisticRegression(multi_class='multinomial')
clf.fit(X, levels)

# Extract coefficients and update in .ino file
```

## Power Consumption

**Typical Values:**
- **STM32:** ~50-100mA (with OLED, active sensors)
- **ESP8266:** ~100-200mA (WiFi active)
- **Total:** ~150-300mA at 5V

**For Extended Battery Operation:**
- Reduce sensor read interval
- Implement sleep modes during low-rain periods
- Use solar charging for sustained operation

## Security Considerations

⚠️ **Development Setup (Current)**
- Firebase rules: Public (not secure)
- WiFi password visible in code
- SSL certificate verification disabled

✅ **For Production Deployment**
- Set Firebase rules to restrict access
- Store credentials in EEPROM/secure storage
- Enable SSL certificate pinning
- Use OAuth 2.0 for app authentication

---

**For questions or issues, check the debug serial output on both devices.**
**Last Updated: 2026-03-22**
