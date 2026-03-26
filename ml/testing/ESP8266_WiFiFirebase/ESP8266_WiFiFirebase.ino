#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>

// ===== WIFI =====
const char* ssid = "";
const char* password = "";

// ===== FIREBASE HOST =====
const char* host = "";

// ===== SENSOR VARIABLES =====
float temperature = 0;
float humidity = 0;
int maxSNR = 0;
int rainValue = 0;
int rainLevel = 0;

bool newData = false;

// ===== TIME FUNCTION =====
String getTimeString()
{
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  char buf[25];
  sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",
          t->tm_year+1900,
          t->tm_mon+1,
          t->tm_mday,
          t->tm_hour,
          t->tm_min,
          t->tm_sec);

  return String(buf);
}

void setup()
{
  Serial.begin(9600);   // UART from STM32

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  // ===== NTP TIME SYNC =====
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
}

void loop()
{
  if (Serial.available())
  {
    String line = Serial.readStringUntil('\n');
    line.trim();

    if(line.startsWith("DATA,"))
    {
      parseData(line);
      newData = true;
    }
  }

  if(newData)
  {
    sendFirebase();
    newData = false;
  }
}

void parseData(String s)
{
  float t,h;
  int snr,rain,level;

  sscanf(s.c_str(),"DATA,%f,%f,%d,%d,%d",&t,&h,&snr,&rain,&level);

  temperature = t;
  humidity = h;
  maxSNR = snr;
  rainValue = rain;
  rainLevel = level;
}

void sendFirebase()
{
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;

  String url = String("https://") + host + "/RainData.json";

  if (https.begin(client, url))
  {
    https.addHeader("Content-Type","application/json");

    String json = "{";
    json += "\"created_at\":\"" + getTimeString() + "\",";
    json += "\"temperature\":" + String(temperature,2) + ",";
    json += "\"humidity\":" + String(humidity,2) + ",";
    json += "\"maxSNR\":" + String(maxSNR) + ",";
    json += "\"rainValue\":" + String(rainValue) + ",";
    json += "\"rainLevel\":" + String(rainLevel);
    json += "}";

    https.POST(json);
    https.end();
  }
}