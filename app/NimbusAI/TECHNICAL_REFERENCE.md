# NimbusAI Technical Reference

## System Components Overview

### 1. STM32 Microcontroller (STM32_RainfallMonitor.ino)

**Purpose:** Sensor data acquisition, processing, and rain prediction

**Serial Ports Configuration:**
- **Serial (9600 baud):** Debug console
- **Serial1 (9600 baud):** GPS module input ($GPGSV sentences)
- **Serial2 (115200 baud):** ESP8266 communication

**Data Processing Pipeline:**
```
┌──────────────┐
│ Read Sensors │
│ (2s interval)│
└──────┬───────┘
       │
       ▼
┌──────────────────┐
│ Temperature      │ DHT11
│ Humidity         │
│ Rain Value       │ Analog sensor
│ SNR Values       │ GPS Parser
└──────┬───────────┘
       │
       ▼
┌──────────────────┐     
│ SNR Filtering    │ Remove > 60
│ Calculate Max SNR│
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ ML Prediction    │ 3-class classifier
│ (z0, z1, z2)    │
└──────┬───────────┘
       │
       ├─────→ Display on OLED
       │
       └─────→ Send to ESP8266 via Serial2
```

**Important Functions:**

| Function | Purpose | Call Interval |
|----------|---------|--------------|
| `readAllSensors()` | Reads DHT11, rain sensor, GPS | 2000ms |
| `readGPSSNR()` | Parses GPS $GPGSV data | 2000ms |
| `getMaxSNR()` | Calculates max SNR (1-60 only) | Every read |
| `predictRain()` | ML model prediction | Every read |
| `updateDisplay()` | OLED refresh | 2000ms |
| `sendToESP8266()` | Serial transmission | 10000ms |

**Serial Data Format (to ESP8266):**
```
TEMPERATURE,HUMIDITY,MAXSNR,RAINVALUE,RAINLEVEL\n

Example:
28.50,65.20,42,512,1\n
└─┬──┘ └────┘ └──┘ └──┘ └─┘
  │     │      │    │    └─ Rain Level (0/1/2)
  │     │      │    └────── Rain Value (0-1023)
  │     │      └─────────── Max SNR (1-60)
  │     └─────────────────── Humidity %
  └────────────────────────── Temperature °C
```

**Memory Usage:**
- SNR buffer: 20 integers (80 bytes)
- Sensor data: ~30 bytes
- Display buffer: internal to Adafruit library
- **Total:** ~1KB RAM

### 2. ESP8266 WiFi Module (ESP8266_WiFiFirebase.ino)

**Purpose:** WiFi connectivity, Firebase uploads, mobile app server

**Serial Port Configuration:**
- **Serial (115200 baud):** Debug console & STM32 communication

**Data Flow:**
```
┌──────────────────┐
│ Receive from STM32│
│ (Serial input)   │
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│ Parse Serial Data│ Parse comma-separated values
└────────┬─────────┘
         │
         ├──────→ Store in currentData struct
         │
         ├──────→ Serve via /data API
         │
         └──────→ Send to Firebase (10s interval)
              │
              ▼
         ┌──────────────────┐
         │ Build JSON       │ Add timestamp
         │ Payload          │
         └────────┬─────────┘
                  │
                  ▼
         ┌──────────────────┐
         │ HTTPS POST to    │
         │ Firebase RTDB    │
         └──────────────────┘
```

**Important Functions:**

| Function | Purpose | Call Trigger |
|----------|---------|--------------|
| `parseSerialData()` | Extract comma-separated values | On serial available |
| `getFormattedTimestamp()` | NTP-synced timestamp | Before each upload |
| `buildFirebasePayload()` | Create JSON for Firebase | Before upload |
| `sendToFirebase()` | POST to Firebase | 10000ms interval |
| `handleDataAPI()` | Web server /data endpoint | On HTTP request |

**Web Server Endpoints:**

**GET /data**
```
Request: GET http://<ESP8266_IP>/data

Response (200 OK):
{
  "created_at": "2026-03-22 14:30",
  "temperatur": 28.50,
  "humidity": 65.20,
  "maxSNR": 42,
  "rainValue": 512,
  "rainLevel": 1
}

Headers:
- Content-Type: application/json
- Access-Control-Allow-Origin: *
```

**GET /status**
```
Request: GET http://<ESP8266_IP>/status

Response (200 OK):
{
  "status": "connected",
  "wifi": true
}
```

**Firebase Upload Path:**
```
POST https://rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app/RainData.json

Response (200 OK):
{
  "name": "-N12abc34def56..."  // Firebase-generated key
}
```

**Memory Usage:**
- currentData struct: ~50 bytes
- JSON payload: ~200 bytes max
- HTTP buffer: handled by ESP8266 lib
- **Total:** ~5KB RAM available

## Communication Protocol Details

### Serial Protocol Between STM32 and ESP8266

**Physical Connection:**
```
STM32 Pin PA2 (TX on Serial2) ──→ ESP8266 RX
STM32 Pin PA3 (RX on Serial2) ←── ESP8266 TX
        (3.3V logic level)
```

**Protocol Specification:**
- **Baud Rate:** 115200
- **Data Bits:** 8
- **Stop Bits:** 1
- **Parity:** None
- **Flow Control:** None

**Frame Format:**
```
[FIELD1],[FIELD2],[FIELD3],[FIELD4],[FIELD5]\n
```

**Timing:**
- STM32 sends every 10 seconds
- ESP8266 processes immediately upon receive
- No handshake (fire-and-forget)
- Timeout: 5 seconds before ESP8266 logs warning

**Error Handling:**
- Invalid format: Skip frame, log error
- Missing fields: Skip frame, log error
- Non-numeric values: Skip field, use default (0)

### Firebase Data Structure

**Database Path:** `https://rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app/RainData`

**Auto-generated Keys:**
Each upload creates a new child with Firebase-generated key:
```
/RainData/{key1}/{field}
/RainData/{key2}/{field}
/RainData/{key3}/{field}
```

**To query latest data in app:**
```
Query: Order by timestamp DESC, limit 1
```

**Retention:** Unlimited (consider archiving old data quarterly)

## ML Model Specification

### Rain Level Classification

**Input Variables:**
- `T`: Temperature (°C) - range 15-45
- `H`: Humidity (%) - range 30-100
- `SNR`: Max Satellite SNR - range 1-60
- `Rain`: Rain sensor value - range 0-1023

**Model Type:** Linear discriminant analysis (3-class softmax)

**Classes:**
```
Class 0: No Rain (p > 0.5)
Class 1: Low Rain (0.3 < p < 0.7)
Class 2: High Rain (p > 0.5)
```

**Coefficients (DO NOT MODIFY without retraining):**
```cpp
// Class 0 (No Rain)
z0 = (11.03188626 * T) +     // Positive correlation: higher temp = more likely no rain
     (-16.75532045 * H) +    // Negative correlation: higher humidity = more likely rain
     (4.14548722 * SNR) +    // Positive correlation: better signal = more data reliability
     (-8.36817677 * Rain) +  // Negative correlation: more rain = high sensor value
     1.32955736;             // Bias term

// Class 1 (Low Rain)
z1 = (0.43147848 * T) +
     (3.77895846 * H) +
     (0.76796779 * SNR) +
     (-3.00623068 * Rain) +
     0.12708801;

// Class 2 (High Rain)
z2 = (-11.46336474 * T) +
     (12.97636199 * H) +
     (-4.91345502 * SNR) +
     (11.37440745 * Rain) +
     (-1.45664538);
```

**Decision Logic:**
```
argmax(z0, z1, z2) → predicted class
```

**Training Data Requirements (for retraining):**
- Minimum 100 samples per class
- Balanced dataset (similar samples per class)
- Include edge cases (sensor failures, extreme weather)

## Troubleshooting & Debug Guide

### Common Issues & Solutions

#### Serial Communication Issues

**Problem:** ESP8266 not receiving data from STM32
1. Check baud rates match (115200 on ESP8266 Serial, Serial2 on STM32)
2. Verify TX/RX wires not crossed
3. Check GND connection (needs common ground)
4. Look at TX LED on STM32 (should blink every 10s)

**Debug:** Monitor both serial ports
```
STM32: COM5 at 9600 - should show "Sent to ESP8266: ..."
ESP8266: COM6 at 115200 - should show "Data parsed successfully..."
```

#### WiFi Connection Issues

**Problem:** "WiFi not connected!" message
1. Verify SSID and password in code match your network
2. Check ESP8266 antenna connection
3. Restart ESP8266 and router
4. Check router WiFi 2.4GHz band enabled (ESP8266 doesn't support 5GHz)

**Debug:**
```
Serial output: "Connecting to WiFi: Galaxy" then dots
Expected: "Connected!" + IP address within 10 seconds
```

#### Firebase Upload Issues

**Problem:** Firebase shows 401/403 errors
1. Verify Firebase URL is correct (no extra slashes)
2. Check database rules allow public write (development only)
3. Verify internet connectivity first

**Problem:** Firebase shows 400 errors
1. Check JSON payload format (extra commas, wrong types)
2. Verify "created_at" timestamp format: "YYYY-MM-DD HH:MM"

**Debug:**
```cpp
// Add to ESP8266 code to see full response:
if (response > 0) {
  String body = http.getString();
  Serial.println("Response: " + body);
}
```

#### SNR Filtering Issues

**Problem:** MaxSNR always 0
1. GPS module not receiving satellite data
2. $GPGSV sentences not being parsed
3. All SNR values above 60

**Debug on STM32:**
```
Serial Monitor should show:
"SNR: 42, SNR: 51, SNR: 38, MaxSNR: 51"
```

#### OLED Display Issues

**Problem:** OLED showing nothing
1. Check I2C address (default 0x3C)
2. Run I2C scanner to verify address
3. Check SDA/SCL wiring
4. Verify 3.3V power supply

**Problem:** Display flickering
1. Reduce display update rate
2. Check for noise in power lines
3. Add capacitors (100nF + 10uF) near display

## Performance Metrics

### Latency
- **Sensor Read → OLED:** ~200ms
- **STM32 → ESP8266:** ~100ms (serial transmission)
- **Parse → Firebase:** ~500ms (network dependent)
- **Firebase → Mobile App:** ~1-2s (Firebase listener lag)
- **Total E2E Latency:** ~5-10 seconds

### Throughput
- **Data Rate:** 1 complete reading every 10 seconds
- **Firebase Uploads:** 8640 per day (1 every 10s)
- **Firebase Storage:** ~10MB per month (at 1KB per upload)

### Accuracy
- **Temperature:** ±0.5°C (DHT11 spec)
- **Humidity:** ±5% (DHT11 spec)
- **SNR:** ±1 dB (GPS receiver dependent)
- **Rain Prediction:** 85-92% (depends on training data)

## Power Management

### Current Draw
| Component | Current | Notes |
|-----------|---------|-------|
| STM32 | 30-50mA | Active |
| DHT11 | 2-3mA | Low |
| OLED | 20-30mA | Active display |
| GPS | 20-30mA | Low power mode |
| ESP8266 (WiFi TX) | 200-300mA | Peak |
| ESP8266 (WiFi RX) | 80-100mA | Idle |

### Optimization Strategies
1. **Reduce sensor read interval** (currently 2s, can be 5s)
2. **Disable OLED during idle** (save 20-30mA)
3. **Lower ESP8266 TX power** (WiFi settings)
4. **Use sleep modes** (RTC + wake on interrupt)

## Expansion Options

### Additional Sensors
- **Pressure Sensor (BMP180):** Add barometric data
- **Light Sensor (LDR):** Detect cloud coverage
- **Wind Speed:** Integrate anemometer via pin interrupt
- **Lightning Detector:** Add weather alerts

### Data Export
- **SD Card Module:** Local data logging
- **LoRa Module:** Remote transmission without WiFi
- **Bluetooth:** Direct mobile connection

### Model Updates
- **Remote firmware update:** Over-the-air updates for model coefficients
- **Edge ML:** Run TensorFlow Lite models on ESP32

---

**Document Version:** 1.0  
**Last Updated:** 2026-03-22  
**Maintainer:** NimbusAI Development Team
