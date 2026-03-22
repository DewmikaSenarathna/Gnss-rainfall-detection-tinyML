/*
  STM32 Rainfall Monitor with Sensor Integration
  - Reads temperature and humidity (DHT11)
  - Reads rain sensor (analog)
  - Parses GNSS satellite SNR values
  - Filters SNR: removes values > 60, keeps others
  - Predicts rain intensity using ML model (0=No Rain, 1=Low Rain, 2=High Rain)
  - Displays data on 128x32 OLED
  - Sends data to ESP8266 via SoftwareSerial (TX/RX)
  
  Hardware Setup:
  - DHT11: Pin 2
  - Rain Sensor: A0
  - GPS Module: Serial1 (9600 baud)
  - OLED: I2C (0x3C)
  - ESP8266: SoftwareSerial on PB10 (RX) and PB11 (TX) at 9600 baud
    (Adjust pins below based on your available GPIO)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// ==================== SOFTWARE SERIAL FOR ESP8266 ====================
// RX pin on STM32, TX pin on STM32 (connected to ESP8266 TX, RX respectively)
SoftwareSerial espSerial(PB10, PB11); // RX on PB10, TX on PB11 (adjust to your pins)

// ==================== FUNCTION PROTOTYPES ====================
void readAllSensors();
void readGPSSNR();
void parseSNRFromGPGSV(const String& sentence);
int getMaxSNR();
int predictRain(float T, float H, float SNR, float Rain);
void updateDisplay();
void sendToESP8266();
bool isFiniteNumber(float value);
void drawBootMessage(const String& line1, const String& line2);

// ==================== OLED DISPLAY ====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==================== DHT11 SENSOR ====================
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==================== RAIN SENSOR ====================
#define RAIN_SENSOR A0

// ==================== GPS SNR BUFFER ====================
int snrValues[20];
int snrCount = 0;

// ==================== SENSOR DATA ====================
float temperature = 0.0;
float humidity = 0.0;
int rainValue = 0;
int maxSNR = 0;
int rainLevel = 0;
bool displayReady = false;
uint8_t oledAddress = 0x3C;
bool hasValidDht = false;

// ==================== TIMING ====================
unsigned long lastSensorReadTime = 0;
unsigned long lastDisplayUpdateTime = 0;
unsigned long lastWiFiSendTime = 0;

const unsigned long SENSOR_READ_INTERVAL = 2000;   // Read sensors every 2 seconds
const unsigned long DISPLAY_UPDATE_INTERVAL = 2000; // Update display every 2 seconds
const unsigned long WIFI_SEND_INTERVAL = 10000;    // Send to WiFi every 10 seconds

// ==================== SETUP ====================
void setup() {
  // Main Serial for debugging (9600 baud)
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n\n========== STM32 Rainfall Monitor Starting ==========");
  
  // GPS Serial (GPS module data - 9600 baud)
  Serial1.begin(9600);
  Serial.println("GPS Serial initialized");
  
  // Software Serial to ESP8266 (9600 baud for stable SoftwareSerial)
  espSerial.begin(9600);
  Serial.println("WiFi Serial (to ESP8266) initialized via SoftwareSerial");
  
  // Initialize DHT11
  dht.begin();
  Serial.println("DHT11 initialized");
  
  // Initialize Rain Sensor
  pinMode(RAIN_SENSOR, INPUT);
  Serial.println("Rain sensor initialized");
  
  // Initialize OLED Display (try 0x3C first, then 0x3D)
  Wire.begin();
  displayReady = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (!displayReady) {
    oledAddress = 0x3D;
    displayReady = display.begin(SSD1306_SWITCHCAPVCC, oledAddress);
  }

  if (displayReady) {
    drawBootMessage("NimbusAI", "Display Ready");
    Serial.print("OLED initialized at 0x");
    Serial.println(oledAddress, HEX);
  } else {
    Serial.println("OLED initialization failed (0x3C/0x3D). Continuing without OLED.");
  }
  
  delay(2000);
}

// ==================== MAIN LOOP ====================
void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensors at regular interval
  if (currentMillis - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    lastSensorReadTime = currentMillis;
  }
  
  // Update display at regular interval
  if (currentMillis - lastDisplayUpdateTime >= DISPLAY_UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdateTime = currentMillis;
  }
  
  // Send data to WiFi module at regular interval
  if (currentMillis - lastWiFiSendTime >= WIFI_SEND_INTERVAL) {
    sendToESP8266();
    lastWiFiSendTime = currentMillis;
  }
}

// ==================== READ ALL SENSORS ====================
void readAllSensors() {
  // Read DHT11 (temperature & humidity)
  const float t = dht.readTemperature();
  const float h = dht.readHumidity();

  // Keep last valid values if current DHT read is invalid.
  if (isFiniteNumber(t)) {
    temperature = t;
  }
  if (isFiniteNumber(h)) {
    humidity = h;
  }
  if (isFiniteNumber(t) && isFiniteNumber(h)) {
    hasValidDht = true;
  }
  
  // Read rain sensor (0-1023)
  rainValue = analogRead(RAIN_SENSOR);
  
  // Read GPS SNR values
  readGPSSNR();
  
  // Get max SNR (filters out values > 60)
  maxSNR = getMaxSNR();
  
  // Predict rain level using ML model
  rainLevel = predictRain(temperature, humidity, maxSNR, rainValue);
  
  // Debug output
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C | Hum: ");
  Serial.print(humidity);
  Serial.print("% | MaxSNR: ");
  Serial.print(maxSNR);
  Serial.print(" | RainValue: ");
  Serial.print(rainValue);
  Serial.print(" | RainLevel: ");
  Serial.println(rainLevel);
}

// ==================== READ GPS SNR ====================
void readGPSSNR() {
  snrCount = 0; // Reset SNR count for new cycle
  
  // Read available GPS data
  while (Serial1.available()) {
    String line = Serial1.readStringUntil('\n');
    line.trim();
    
    // Parse GPGSV sentences (satellite info with SNR)
    if (line.startsWith("$GPGSV")) {
      parseSNRFromGPGSV(line);
    }
  }
}

// ==================== PARSE SNR FROM GPGSV ====================
void parseSNRFromGPGSV(const String& sentence) {
  // GPGSV format: $GPGSV,msg,msgid,numSV,sat1,elev1,azim1,SNR1[,sat2,elev2,azim2,SNR2]...
  // SNR values are at positions 7, 11, 15, 19 (comma-separated)
  
  int commaCount = 0;
  for (int i = 0; i < sentence.length() && snrCount < 20; i++) {
    if (sentence.charAt(i) == ',') {
      commaCount++;
      
      // SNR values are at positions 7, 11, 15, 19
      if (commaCount == 7 || commaCount == 11 || commaCount == 15 || commaCount == 19) {
        // Extract SNR value (next 2-3 characters after comma)
        int snrStart = i + 1;
        int snrEnd = snrStart;
        
        while (snrEnd < sentence.length() && sentence.charAt(snrEnd) != ',' && sentence.charAt(snrEnd) != '\n') {
          snrEnd++;
        }
        
        String snrStr = sentence.substring(snrStart, snrEnd);
        int snr = snrStr.toInt();
        
        // Filter: only keep SNR values between 1 and 60
        if (snr > 0 && snr <= 60) {
          snrValues[snrCount++] = snr;
        }
      }
    }
  }
}

// ==================== GET MAX SNR ====================
int getMaxSNR() {
  int maxVal = 0;
  
  for (int i = 0; i < snrCount; i++) {
    if (snrValues[i] > maxVal) {
      maxVal = snrValues[i];
    }
  }
  
  return maxVal;
}

// ==================== ML MODEL: PREDICT RAIN ====================
// Softmax-like rain prediction model trained on historical sensor data
// Returns: 0 = No Rain, 1 = Low Rain, 2 = High Rain
int predictRain(float T, float H, float SNR, float Rain) {
  // Model coefficients (optimized for your sensor ranges)
  float z0 = (11.03188626 * T) + (-16.75532045 * H) + (4.14548722 * SNR) + (-8.36817677 * Rain) + 1.32955736;
  float z1 = (0.43147848 * T) + (3.77895846 * H) + (0.76796779 * SNR) + (-3.00623068 * Rain) + 0.12708801;
  float z2 = (-11.46336474 * T) + (12.97636199 * H) + (-4.91345502 * SNR) + (11.37440745 * Rain) - 1.45664538;
  
  // Return class with highest score
  if (z0 > z1 && z0 > z2) return 0; // No Rain
  if (z1 > z0 && z1 > z2) return 1; // Low Rain
  return 2; // High Rain
}

// ==================== UPDATE OLED DISPLAY ====================
void updateDisplay() {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  // Line 1: Temperature
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println("C");
  
  // Line 2: Humidity
  display.print("Hum: ");
  display.print(humidity, 1);
  display.println("%");
  
  // Line 3: Max SNR
  display.print("MaxSNR: ");
  display.println(maxSNR);
  
  // Line 4: Rain Level
  display.print("Rain: ");
  if (rainLevel == 0) {
    display.println("No Rain");
  } else if (rainLevel == 1) {
    display.println("Low Rain");
  } else if (rainLevel == 2) {
    display.println("High Rain");
  }
  
  display.display();
}

// ==================== SEND DATA TO ESP8266 ====================
void sendToESP8266() {
  if (!hasValidDht) {
    Serial.println("Skipping send: waiting for valid DHT data.");
    return;
  }

  // Format: DATA,TEMP,HUMIDITY,MAXSNR,RAINVALUE,RAINLEVEL\n
  // Example: DATA,28.50,65.20,42,512,1\n
  
  String payload = "";
  payload += String(temperature, 2) + ",";
  payload += String(humidity, 2) + ",";
  payload += String(maxSNR) + ",";
  payload += String(rainValue) + ",";
  payload += String(rainLevel);
  
  // Frame marker helps ESP8266 ignore noisy/partial lines.
  payload = "DATA," + payload;

  // Send via SoftwareSerial to ESP8266
  espSerial.println(payload);
  
  Serial.print("Sent to ESP8266: ");
  Serial.println(payload);
}

bool isFiniteNumber(float value) {
  return !isnan(value) && !isinf(value);
}

void drawBootMessage(const String& line1, const String& line2) {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
  display.setCursor(0, 12);
  display.println(line2);
  display.display();
}
