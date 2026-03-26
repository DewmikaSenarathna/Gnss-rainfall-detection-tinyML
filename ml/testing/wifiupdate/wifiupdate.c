#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ===== UART =====
HardwareSerial espSerial(PB11, PB10);   // RX , TX

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== Sensors =====
#define DHTPIN PA1
#define DHTTYPE DHT11
#define RAIN_SENSOR PA0

DHT dht(DHTPIN, DHTTYPE);

// ===== GPS SNR =====
int snrValues[20];
int snrCount = 0;
int maxSNR = 0;
float avgSNR = 0;

// ===== Sensor Data =====
float temperature = 0;
float humidity = 0;
int rainValue = 0;
int rainLevel = 0;

unsigned long lastUpdate = 0;

String gpsLine = "";

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);     // GPS
  espSerial.begin(9600);

  dht.begin();
  pinMode(RAIN_SENSOR, INPUT);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("NimbusAI");
  display.println("Rain Monitor");
  display.display();

  delay(2000);
}

void loop()
{
  readGPS_continuous();  

  if(millis() - lastUpdate > 2000)
  {
    readSensors();
    calcSNR();
    predictRain();
    updateDisplay();
    sendESP();

    lastUpdate = millis();
  }
}

void readGPS_continuous()
{
  while(Serial1.available())
  {
    char c = Serial1.read();

    if(c == '\n')
    {
      if(gpsLine.startsWith("$GPGSV") ||
         gpsLine.startsWith("$GLGSV") ||
         gpsLine.startsWith("$GNGSV"))
      {
        parseSNR(gpsLine);
      }
      gpsLine = "";
    }
    else
    {
      gpsLine += c;
    }
  }
}

void parseSNR(String sentence)
{
  int comma = 0;

  for(int i=0;i<sentence.length();i++)
  {
    if(sentence[i]==',')
    {
      comma++;

      if(comma==7 || comma==11 || comma==15 || comma==19)
      {
        int snr = sentence.substring(i+1, i+3).toInt();

        if(snr>0 && snr<=60 && snrCount<20)
          snrValues[snrCount++] = snr;
      }
    }
  }
}

void calcSNR()
{
  int total = 0;
  maxSNR = 0;

  for(int i=0;i<snrCount;i++)
  {
    total += snrValues[i];
    if(snrValues[i] > maxSNR)
      maxSNR = snrValues[i];
  }

  if(snrCount>0)
    avgSNR = (float)total/snrCount;
  else
    avgSNR = 0;

  snrCount = 0;   
}

void readSensors()
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if(!isnan(t)) temperature = t;
  if(!isnan(h)) humidity = h;

  rainValue = analogRead(RAIN_SENSOR);

  Serial.print("T=");
  Serial.print(temperature);
  Serial.print(" H=");
  Serial.print(humidity);
  Serial.print(" Rain=");
  Serial.print(rainValue);
  Serial.print(" SNR=");
  Serial.println(maxSNR);
}

void predictRain()
{
  float z0 = (11.03*temperature) + (-16.75*humidity) + (4.14*maxSNR) + (-0.008*rainValue);
  float z1 = (0.43*temperature) + (3.77*humidity) + (0.76*maxSNR) + (-0.003*rainValue);
  float z2 = (-11.46*temperature) + (12.97*humidity) + (-4.91*maxSNR) + (0.011*rainValue);

  if(z0>z1 && z0>z2) rainLevel = 0;
  else if(z1>z0 && z1>z2) rainLevel = 1;
  else rainLevel = 2;
}

void updateDisplay()
{
  display.clearDisplay();
  display.setCursor(0,0);

  display.printf("T:%.1fC H:%.0f%%\n", temperature, humidity);
  display.printf("SNR:%d Rain:%d\n", maxSNR, rainValue);

  display.print("Level:");
  if(rainLevel==0) display.println("No");
  else if(rainLevel==1) display.println("Low");
  else display.println("High");

  display.display();
}

void sendESP()
{
  espSerial.print("DATA,");
  espSerial.print(temperature);
  espSerial.print(",");
  espSerial.print(humidity);
  espSerial.print(",");
  espSerial.print(maxSNR);
  espSerial.print(",");
  espSerial.print(rainValue);
  espSerial.print(",");
  espSerial.println(rainLevel);

  Serial.println("Sent ESP");
}