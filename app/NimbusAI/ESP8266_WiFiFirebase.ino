/*
  ESP8266 WiFi & Firebase Module
  - Receives sensor data from STM32 via SoftwareSerial (9600 baud)
  - Connects to WiFi
  - Sends data to Firebase Realtime Database with timestamp
  - Serves local web API for mobile app real-time updates
  - Maintains continuous communication loop
  
  Hardware Setup:
  - D6 (RX) connected to STM32 TX pin
  - D5 (TX) connected to STM32 RX pin
  - Baud rate: 9600
  
  Firebase Paths: /RainData/latest and /RainData/history
  Data Structure:
  {
    "created_at": "2026-03-22 14:30",
    "temperatur": 28.50,
    "humidity": 65.20,
    "maxSNR": 42,
    "rainValue": 512,
    "rainLevel": 1
  }
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <SoftwareSerial.h>

// ==================== WIFI CONFIGURATION ====================
const char* ssid = "Galaxy";
const char* password = "dondew0027";

// ==================== FIREBASE CONFIGURATION ====================
const char* firebaseHost = "rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* firebaseAuth = "";  // Add DB secret if required
const char* firebaseLatestPath = "/RainData/latest.json";
const char* firebaseHistoryPath = "/RainData/history.json";

// ==================== STM32 SERIAL (SOFTWARE SERIAL) ====================
// Uses D6/D5 aliases when available (NodeMCU/Wemos), else falls back to GPIO numbers.
#if defined(D6)
const uint8_t STM32_RX_PIN = D6;  // ESP RX from STM32 TX
#else
const uint8_t STM32_RX_PIN = 12;  // GPIO12
#endif

#if defined(D5)
const uint8_t STM32_TX_PIN = D5;  // ESP TX to STM32 RX
#else
const uint8_t STM32_TX_PIN = 14;  // GPIO14
#endif

SoftwareSerial stmSerial(STM32_RX_PIN, STM32_TX_PIN);

// ==================== WEB SERVER ====================
ESP8266WebServer server(80);

// ==================== SENSOR DATA STORAGE ====================
struct SensorData {
  float temperature;
  float humidity;
  int maxSNR;
  int rainValue;
  int rainLevel;
  String createdAt;
} currentData;

String latestPayload = "{}";
bool hasValidSensorData = false;

// ==================== TIMING ====================
unsigned long lastFirebaseSendTime = 0;
const unsigned long FIREBASE_SEND_INTERVAL = 10000; // Send to Firebase every 10 seconds
const unsigned long SERIAL_READ_TIMEOUT = 5000;     // Timeout for serial read

// ==================== FUNCTION PROTOTYPES ====================
String getFormattedTimestamp();
String buildFirebasePayload();
void sendToFirebase();
void handleDataAPI();
void parseSerialData(String data);
void printSensorData();
bool postJson(const String& path, const String& payload, bool usePostMethod);

// ==================== SETUP ====================
void setup() {
  // Debug Serial (115200 baud)
  Serial.begin(115200);
  delay(1000);

  // Dedicated UART to receive STM32 payload without USB serial conflicts.
  stmSerial.begin(9600);
  
  Serial.println("\n\n========== ESP8266 WiFi/Firebase Module Starting ==========");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
  }
  
  // Configure NTP time (UTC+5:30 for your timezone)
  // Change offset if needed: UTC hours * 3600 + UTC minutes * 60
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
  
  Serial.println("Waiting for NTP time sync...");
  time_t now = time(nullptr);
  int timeAttempts = 0;
  while (now < 24 * 3600 && timeAttempts < 20) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    timeAttempts++;
  }
  Serial.println();
  
  // Setup web server endpoints
  server.on("/data", handleDataAPI);
  server.on("/status", HTTP_GET, []() {
    String status = "{\"status\":\"connected\",\"wifi\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + "}";
    server.send(200, "application/json", status);
  });
  
  server.begin();
  Serial.println("Web server started on port 80");
  
  // Initialize sensor data
  currentData.temperature = 0.0;
  currentData.humidity = 0.0;
  currentData.maxSNR = 0;
  currentData.rainValue = 0;
  currentData.rainLevel = 0;
  currentData.createdAt = getFormattedTimestamp();
  
  Serial.println("Setup complete. Waiting for STM32 data...");
}

// ==================== MAIN LOOP ====================
void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read data from STM32 via SoftwareSerial
  if (stmSerial.available()) {
    String rawData = stmSerial.readStringUntil('\n');
    rawData.trim();
    
    if (rawData.length() > 0) {
      parseSerialData(rawData);
      printSensorData();
    }
  }
  
  // Send to Firebase at regular interval
  unsigned long currentMillis = millis();
  if (currentMillis - lastFirebaseSendTime >= FIREBASE_SEND_INTERVAL) {
    if (WiFi.status() == WL_CONNECTED && hasValidSensorData) {
      sendToFirebase();
    } else if (!hasValidSensorData) {
      Serial.println("No valid STM32 packet yet. Skipping Firebase send.");
    } else {
      Serial.println("WiFi not connected. Skipping Firebase send.");
    }
    lastFirebaseSendTime = currentMillis;
  }
}

// ==================== PARSE SERIAL DATA FROM STM32 ====================
// Format: DATA,TEMP,HUMIDITY,MAXSNR,RAINVALUE,RAINLEVEL
// Example: DATA,28.50,65.20,42,512,1
void parseSerialData(String data) {
  if (!data.startsWith("DATA,")) {
    Serial.println("Ignoring non-data serial line.");
    return;
  }

  data.remove(0, 5); // Strip "DATA,"

  int commaIndex[4];
  int commaCount = 0;
  
  // Find all comma positions
  for (int i = 0; i < data.length() && commaCount < 4; i++) {
    if (data.charAt(i) == ',') {
      commaIndex[commaCount] = i;
      commaCount++;
    }
  }
  
  if (commaCount < 4) {
    Serial.println("ERROR: Invalid data format from STM32");
    return;
  }
  
  // Parse each field
  currentData.temperature = data.substring(0, commaIndex[0]).toFloat();
  currentData.humidity = data.substring(commaIndex[0] + 1, commaIndex[1]).toFloat();
  currentData.maxSNR = data.substring(commaIndex[1] + 1, commaIndex[2]).toInt();
  currentData.rainValue = data.substring(commaIndex[2] + 1, commaIndex[3]).toInt();
  currentData.rainLevel = data.substring(commaIndex[3] + 1).toInt();
  // Basic sanity checks to avoid storing corrupted frames.
  const bool sane =
      currentData.temperature > -40.0 && currentData.temperature < 100.0 &&
      currentData.humidity >= 0.0 && currentData.humidity <= 100.0 &&
      currentData.maxSNR >= 0 && currentData.maxSNR <= 60 &&
      currentData.rainValue >= 0 && currentData.rainValue <= 4095 &&
      currentData.rainLevel >= 0 && currentData.rainLevel <= 2;

  if (!sane) {
    Serial.println("ERROR: Out-of-range values in STM32 packet. Frame discarded.");
    return;
  }

  currentData.createdAt = getFormattedTimestamp();
  hasValidSensorData = true;
  Serial.println("Data parsed successfully from STM32");
}

// ==================== GET FORMATTED TIMESTAMP ====================
String getFormattedTimestamp() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  
  if (t == nullptr || t->tm_year < (2020 - 1900)) {
    // Fallback if NTP time not synced yet
    return String("1970-01-01 00:00");
  }
  
  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min);
  
  return String(buf);
}

// ==================== BUILD FIREBASE PAYLOAD ====================
String buildFirebasePayload() {
  // Note: Firebase key "temperatur" maintained to match existing app schema
  String json = "{";
  json += "\"created_at\":\"" + currentData.createdAt + "\",";
  json += "\"temperatur\":" + String(currentData.temperature, 2) + ",";
  json += "\"humidity\":" + String(currentData.humidity, 2) + ",";
  json += "\"maxSNR\":" + String(currentData.maxSNR) + ",";
  json += "\"rainValue\":" + String(currentData.rainValue) + ",";
  json += "\"rainLevel\":" + String(currentData.rainLevel);
  json += "}";
  return json;
}

// ==================== SEND TO FIREBASE ====================
void sendToFirebase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Firebase: WiFi not connected!");
    return;
  }
  
  WiFiClientSecure client;
  client.setInsecure(); // For development. Use cert pinning for production.
  
  String payload = buildFirebasePayload();
  latestPayload = payload;

  Serial.println("\n========== SENDING TO FIREBASE ==========");
  Serial.println("Payload: " + payload);

  // 1) Keep one latest object for real-time mobile app rendering.
  const bool latestOk = postJson(firebaseLatestPath, payload, false);
  // 2) Keep historical stream for later model retraining.
  const bool historyOk = postJson(firebaseHistoryPath, payload, true);

  Serial.print("Latest write: ");
  Serial.println(latestOk ? "OK" : "FAILED");
  Serial.print("History push: ");
  Serial.println(historyOk ? "OK" : "FAILED");

  Serial.println("=========================================\n");
}

bool postJson(const String& path, const String& payload, bool usePostMethod) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  String url = "https://";
  url += firebaseHost;
  url += path;
  if (String(firebaseAuth).length() > 0) {
    url += "?auth=" + String(firebaseAuth);
  }

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  const int responseCode = usePostMethod ? http.POST(payload) : http.PUT(payload);
  Serial.print("Firebase ");
  Serial.print(usePostMethod ? "POST " : "PUT ");
  Serial.print(path);
  Serial.print(" -> ");
  Serial.println(responseCode);

  if (responseCode > 0) {
    String responseBody = http.getString();
    Serial.print("Body: ");
    Serial.println(responseBody);
    http.end();
    return responseCode >= 200 && responseCode < 300;
  }

  Serial.print("HTTP error: ");
  Serial.println(http.errorToString(responseCode));
  http.end();
  return false;
}

// ==================== WEB API: /DATA ENDPOINT ====================
void handleDataAPI() {
  // CORS headers for mobile app access
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  
  if (server.method() == HTTP_OPTIONS) {
    server.send(200);
    return;
  }
  
  String response = buildFirebasePayload();
  server.send(200, "application/json", response);
}

// ==================== DEBUG: PRINT SENSOR DATA ====================
void printSensorData() {
  Serial.println("\n---------- Current Sensor Data ----------");
  Serial.print("Temperature: ");
  Serial.print(currentData.temperature, 2);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(currentData.humidity, 2);
  Serial.println(" %");
  
  Serial.print("Max SNR: ");
  Serial.println(currentData.maxSNR);
  
  Serial.print("Rain Value: ");
  Serial.println(currentData.rainValue);
  
  Serial.print("Rain Level: ");
  if (currentData.rainLevel == 0) {
    Serial.println("No Rain");
  } else if (currentData.rainLevel == 1) {
    Serial.println("Low Rain");
  } else if (currentData.rainLevel == 2) {
    Serial.println("High Rain");
  }
  
  Serial.print("Timestamp: ");
  Serial.println(currentData.createdAt);
  Serial.println("----------------------------------------\n");
}
