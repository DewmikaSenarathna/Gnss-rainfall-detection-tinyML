#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <time.h>

// ---------------- WIFI ----------------
const char* ssid = "Galaxy";
const char* password = "dondew0027";

// ---------------- FIREBASE ----------------
const char* firebaseHost = "rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* firebaseAuth = "";  // If DB rules require auth, add DB secret/token here

// ---------------- SERVER ----------------
ESP8266WebServer server(80);

// ---------------- SAMPLE DATA ----------------
// (Replace later with real sensors)
float temperature = 29.5;
float humidity = 75.0;
int maxSNR = 42;
float rainValue = 0.0;
int rainLevel = 0;

String latestPayload = "{}";

String firebasePushURL() {
  String url = "https://";
  url += firebaseHost;
  url += "/RainData.json";
  if (String(firebaseAuth).length() > 0) {
    url += "?auth=";
    url += firebaseAuth;
  }
  return url;
}

String nowString() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  if (t == nullptr || t->tm_year < (2020 - 1900)) {
    // Fallback if NTP time is not yet available
    return String("1970-01-01 00:") + String((millis() / 60000) % 60);
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

void updateSensorValues() {
  // Demo updates. Replace with your real sensor readings.
  temperature += 0.10;
  humidity += 0.20;
  maxSNR += 1;

  if (maxSNR > 60) {
    maxSNR = 40;
  }

  rainValue = (maxSNR < 47) ? 1.0 : 0.0;
  rainLevel = (rainValue > 0.0) ? 1 : 0;
}

String buildPayload() {
  // Keep key name "temperatur" to match your existing mobile app/database schema.
  String json = "{";
  json += "\"created_at\":\"" + nowString() + "\",";
  json += "\"temperatur\":" + String(temperature, 2) + ",";
  json += "\"humidity\":" + String(humidity, 2) + ",";
  json += "\"maxSNR\":" + String(maxSNR) + ",";
  json += "\"rainValue\":" + String(rainValue, 1) + ",";
  json += "\"rainLevel\":" + String(rainLevel);
  json += "}";
  return json;
}

// ---------------- WEB API ----------------
void handleData() {
  server.send(200, "application/json", latestPayload);
}

// ---------------- FIREBASE ----------------
void sendToFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();  // For development only. Use cert pinning for production.

    HTTPClient http;
    String payload = buildPayload();
    latestPayload = payload;

    http.begin(client, firebasePushURL());
    http.addHeader("Content-Type", "application/json");

    Serial.println("\nSending to Firebase:");
    Serial.println(payload);

    int response = http.POST(payload);

    Serial.print("Firebase Response: ");
    Serial.println(response);

    if (response > 0) {
      String responseBody = http.getString();
      Serial.print("Firebase Body: ");
      Serial.println(responseBody);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // NTP time (UTC+5:30 example). Change offset if needed for your country.
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");

  latestPayload = buildPayload();

  // Local API for mobile app testing in same Wi-Fi network
  server.on("/data", handleData);
  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();

  static unsigned long lastTime = 0;

  if (millis() - lastTime > 10000) {
    updateSensorValues();

    sendToFirebase();

    lastTime = millis();
  }
}