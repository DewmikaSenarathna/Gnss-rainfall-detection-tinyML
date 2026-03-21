#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial gpsSerial(4,3);
SoftwareSerial espSerial(10,9);

#define RAIN_SENSOR A0

String firebaseHost = "YOUR_FIREBASE_HOST";

int snrValues[20];
int snrCount = 0;

float temperature;
float humidity;
int rainValue;

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  espSerial.begin(9600);

  dht.begin();

  pinMode(RAIN_SENSOR,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  connectWiFi();
}

void loop()
{
  readSensors();

  int maxSNR = getMaxSNR();

  int rainLevel = predictRain(
    temperature,
    humidity,
    maxSNR,
    rainValue
  );

  displayData(maxSNR,rainLevel);

  sendToFirebase(
    temperature,
    humidity,
    maxSNR,
    rainValue,
    rainLevel
  );

  delay(20000);
}

void readSensors()
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  rainValue = analogRead(RAIN_SENSOR);

  readGPS();
}

void readGPS()
{
  snrCount = 0;

  while(gpsSerial.available())
  {
    String line = gpsSerial.readStringUntil('\n');

    if(line.startsWith("$GPGSV"))
    {
      parseSNR(line);
    }
  }
}

void parseSNR(String sentence)
{
  int index = 0;

  for(int i=0;i<sentence.length();i++)
  {
    if(sentence.charAt(i)==',')
    {
      index++;

      if(index==7 || index==11 || index==15 || index==19)
      {
        int snr = sentence.substring(i+1,i+3).toInt();

        if(snr>0 && snr<=60 && snrCount<20)
        {
          snrValues[snrCount++] = snr;
        }
      }
    }
  }
}

int getMaxSNR()
{
  int maxVal = 0;

  for(int i=0;i<snrCount;i++)
  {
    if(snrValues[i] > maxVal)
    {
      maxVal = snrValues[i];
    }
  }

  return maxVal;
}

int predictRain(float T,float H,float SNR,float Rain)
{
  float z0 = (11.03188626*T)+(-16.75532045*H)+(4.14548722*SNR)+(-8.36817677*Rain)+1.32955736;
  float z1 = (0.43147848*T)+(3.77895846*H)+(0.76796779*SNR)+(-3.00623068*Rain)+0.12708801;
  float z2 = (-11.46336474*T)+(12.97636199*H)+(-4.91345502*SNR)+(11.37440745*Rain)-1.45664538;

  if(z0>z1 && z0>z2) return 0;
  if(z1>z0 && z1>z2) return 1;

  return 2;
}

void displayData(int maxSNR,int rainLevel)
{
  display.clearDisplay();

  display.setCursor(0,0);
  display.print("MaxSNR:");
  display.println(maxSNR);

  display.print("Temp:");
  display.println(temperature);

  display.print("Hum:");
  display.println(humidity);

  display.print("Rain:");

  if(rainLevel==0)
  display.println("No Rain");

  if(rainLevel==1)
  display.println("Low Rain");

  if(rainLevel==2)
  display.println("High Rain");

  display.display();
}

void connectWiFi()
{
  sendCommand("AT",2000);
  sendCommand("AT+CWMODE=1",2000);
  sendCommand("AT+CWJAP=\"YOUR_WIFI\",\"YOUR_PASSWORD\"",6000);
}

void sendToFirebase(float temp,float hum,int snr,int rain,int pred)
{
  String json = "{";
  json += "\"temperature\":" + String(temp) + ",";
  json += "\"humidity\":" + String(hum) + ",";
  json += "\"max_snr\":" + String(snr) + ",";
  json += "\"rain_sensor\":" + String(rain) + ",";
  json += "\"prediction\":" + String(pred);
  json += "}";

  sendCommand("AT+CIPSTART=\"TCP\",\""+firebaseHost+"\",80",3000);

  String httpRequest = "POST /RainData.json HTTP/1.1\r\n";
  httpRequest += "Host: " + firebaseHost + "\r\n";
  httpRequest += "Content-Type: application/json\r\n";
  httpRequest += "Content-Length: " + String(json.length()) + "\r\n\r\n";
  httpRequest += json;

  sendCommand("AT+CIPSEND="+String(httpRequest.length()),2000);

  espSerial.print(httpRequest);

  delay(3000);

  sendCommand("AT+CIPCLOSE",2000);
}

void sendCommand(String cmd,int wait)
{
  espSerial.println(cmd);
  delay(wait);

  while(espSerial.available())
  {
    Serial.write(espSerial.read());
  }
}