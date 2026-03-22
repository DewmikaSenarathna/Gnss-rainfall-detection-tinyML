# NimbusAI - Deployment & Testing Checklist

## Pre-Deployment Verification

### Hardware Assembly
- [ ] STM32 board selected and identified (e.g., STM32 Nucleo-64)
- [ ] ESP8266 board selected and identified (e.g., NodeMCU)
- [ ] DHT11 sensor connected to STM32 Pin 2
- [ ] Rain sensor connected to STM32 A0
- [ ] GPS module connected to STM32 Serial1 (RX/TX)
- [ ] OLED 128x32 connected to STM32 I2C (SDA/SCL)
- [ ] STM32 Serial2 TX (PA2) connected to ESP8266 RX
- [ ] STM32 Serial2 RX (PA3) connected to ESP8266 TX
- [ ] Common ground between all devices
- [ ] 3.3V power supply verified for STM32 & ESP8266
- [ ] 5V power supply available for DHT11 & rain sensor

### Software Prerequisites
- [ ] Arduino IDE installed (v1.8.19 or later)
- [ ] Board support installed:
  - [ ] STM32 (via Boards Manager)
  - [ ] ESP8266 (esp8266/Arduino)
- [ ] Required libraries installed:
  - [ ] Adafruit GFX
  - [ ] Adafruit SSD1306
  - [ ] DHT sensor library
  - [ ] (ESP8266 libs are built-in)

### Configuration Files
- [ ] WiFi SSID updated in `ESP8266_WiFiFirebase.ino`
- [ ] WiFi password updated in `ESP8266_WiFiFirebase.ino`
- [ ] Firebase URL verified: `rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app`
- [ ] Firebase database rules set to public read/write (development only)

## Compilation & Upload

### STM32 Upload
1. [ ] Open `STM32_RainfallMonitor.ino` in Arduino IDE
2. [ ] Select Board: `STM32 Nucleo-64 (STM32L073RZ)` (or your model)
3. [ ] Select Port: COM port with STM32
4. [ ] Compile: Sketch → Verify/Compile (check for no errors)
5. [ ] Upload: Sketch → Upload
6. [ ] Wait for "Upload complete" message

### ESP8266 Upload
1. [ ] Open `ESP8266_WiFiFirebase.ino` in Arduino IDE
2. [ ] Select Board: `NodeMCU 1.0 (ESP8266-12E)`
3. [ ] Select Port: COM port with ESP8266
4. [ ] Compile: Sketch → Verify/Compile (check for no errors)
5. [ ] Upload: Sketch → Upload
6. [ ] Wait for "Upload complete" message

## Hardware Testing

### STM32 Verification
1. [ ] Open Serial Monitor at **9600 baud**
2. [ ] Verify startup messages:
   - "STM32 Rainfall Monitor Starting"
   - "GPS Serial initialized"
   - "WiFi Serial (to ESP8266) initialized"
   - "DHT11 initialized"
   - "OLED initialized"

3. [ ] Check DHT11 readings:
   - Temperature should be in reasonable range (15-35°C typical)
   - Humidity should be in reasonable range (30-80%)
   - Not all zeros (indicates read failure)

4. [ ] Check rain sensor reading:
   - Should show 0-1023 value
   - Testing: Cover sensor for high value, expose to light for low

5. [ ] Check GPS SNR:
   - Should show variable values 1-60
   - Testing: Check outdoor signal (may need 5-10 min for fix)

6. [ ] Verify OLED display displays all 4 lines:
   - "Temp: XX.XX C"
   - "Hum: XX.XX %"
   - "MaxSNR: XX"
   - "Rain: [No Rain/Low Rain/High Rain]"

7. [ ] Verify serial send every 10 seconds:
   - Should see "Sent to ESP8266: XX.XX,YY.YY,ZZ,WWW,R"

### ESP8266 Verification
1. [ ] Open Serial Monitor at **115200 baud**
2. [ ] Verify startup messages:
   - "ESP8266 WiFi/Firebase Module Starting"
   - "Connecting to WiFi: Galaxy"
   - "WiFi Connected!"
   - "IP Address: 192.168.x.x"

3. [ ] Check data reception:
   - Should see "Data parsed successfully from STM32"
   - Should see "Current Sensor Data" output every 10 seconds

4. [ ] Verify Firebase uploads:
   - Should see "Response Code: 200" (success)
   - Should see Firebase URL being accessed
   - Check Firebase Console for new entries

## Integration Testing

### Serial Communication Test
**Objective:** Verify STM32 ↔ ESP8266 communication

1. [ ] Open two terminal windows
2. [ ] Terminal 1: STM32 at 9600 baud
3. [ ] Terminal 2: ESP8266 at 115200 baud
4. [ ] Watch for:
   - STM32: "Sent to ESP8266: [data]" 
   - ESP8266: "Data parsed successfully"
   - Should happen synchronously

### WiFi Connectivity Test
**Objective:** Verify ESP8266 connects to network

1. [ ] Power on ESP8266
2. [ ] Open router admin panel
3. [ ] Check connected devices
4. [ ] ESP8266 should appear in device list
5. [ ] Verify IP is pingable: `ping 192.168.x.x` (Windows)

### Firebase Database Test
**Objective:** Verify data reaches Firebase

1. [ ] Go to Firebase Console
2. [ ] Select project
3. [ ] Realtime Database → Data tab
4. [ ] Should see `/RainData` node
5. [ ] Should see new entries appearing every 10 seconds
6. [ ] Click entry to verify structure:
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

### Web API Test
**Objective:** Verify ESP8266 web server works

**On your phone or computer on same WiFi:**
1. [ ] Open browser
2. [ ] Go to: `http://<ESP8266_IP>/data`
3. [ ] Should return JSON with current sensor data
4. [ ] Should update every 10 seconds on refresh

**Or use cURL:**
```bash
curl http://192.168.x.x/data
```

### Mobile App Integration Test
**Objective:** Verify data flows to mobile app

1. [ ] Open NimbusAI mobile app
2. [ ] Configure Firebase connection (if needed)
3. [ ] Should see real-time sensor values updating
4. [ ] Check data matches OLED display and web API
5. [ ] Verify timestamps are within last 10 seconds

## Performance Validation

### Data Update Frequency
- [ ] OLED updates every 2 seconds
- [ ] Serial data sent every 10 seconds
- [ ] Firebase updates every 10 seconds
- [ ] Web API returns fresh data within 10s

### Accuracy Checks
- [ ] Temperature readings consistent with room thermometer
- [ ] Humidity readings reasonable for environment
- [ ] Rain sensor responds to physical stimulus
- [ ] SNR values vary with GPS satellite availability
- [ ] Rain level classification matches expected conditions

### Error Resilience
- [ ] System recovers from WiFi disconnect
- [ ] OLED continues updating when Firebase down
- [ ] No crashes or resets during 1-hour operation
- [ ] Serial buffer doesn't overflow
- [ ] Timestamps stay synchronized over time

## Safety Checks

### Power & Voltage
- [ ] 3.3V rail stable (no oscillation)
- [ ] 5V rail stable when sensor loads applied
- [ ] No smoke or burning smell
- [ ] Heat dissipation normal (boards cool)

### Signal Integrity
- [ ] No visible interference in GPS SNR values
- [ ] DHT11 not showing random spikes
- [ ] Serial data not corrupted
- [ ] WiFi signal strong (-40dBm to -60dBm)

### Thermal
- [ ] STM32 normal temp: <50°C
- [ ] ESP8266 normal temp: <60°C
- [ ] Extended operation (>1 hour): No thermal throttling

## Troubleshooting During Testing

### If OLED doesn't show data
```
1. Check I2C address with scanner code
2. Adjust contrast with display.setContrast()
3. Verify 0x3C address in code
4. Check pull-up resistors on SDA/SCL
```

### If no GPS SNR data
```
1. Take GPS module outside (needs satellite view)
2. Wait 5-10 minutes for lock
3. Check GPGSV sentence format on Serial1
4. Consider GPS module power draw
```

### If Firebase uploads fail
```
1. Check WiFi connection first
2. Verify Firebase URL (no extra slashes)
3. Check database rules allow public write
4. Monitor ESP8266 serial for HTTP error codes
```

### If serial data corrupted
```
1. Check baud rate match (115200)
2. Verify TX/RX not shorted
3. Add 1kΩ series resistors on TX line
4. Check USB cable quality
```

## Production Deployment

### Final Checklist
- [ ] All tests passed successfully
- [ ] No error messages in last hour of operation
- [ ] Data consistently flowing to Firebase
- [ ] Mobile app showing real-time updates
- [ ] Backup configuration saved
- [ ] Initial Firebase data backed up
- [ ] System ready for continuous operation

### Monitoring After Deployment
- [ ] Check Firebase database size weekly
- [ ] Monitor WiFi disconnect events
- [ ] Verify ML model accuracy monthly
- [ ] Archive old data quarterly
- [ ] Retrain model with latest data every 3 months

### Maintenance Schedule
- **Daily:** Check mobile app for anomalies
- **Weekly:** Verify Firebase uploads
- **Monthly:** Review sensor calibration
- **Quarterly:** Archive and backup data
- **Biannually:** Check hardware connections

---

**Testing Notes:**
```
Date Started: _______________
Testing Completed: _______________
Issues Found: _______________
Deployed: _______________
First Production Data: _______________
```

**Tester Name:** _____________________  
**Sign-Off:** ________________________  
**Date:** ____________________________
