#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PINS
#define GPS_RX 4
#define GPS_TX 3
#define SD_CS 10

#define DHTPIN 5
#define DHTTYPE DHT11

#define RAIN_ANALOG A0
#define RAIN_DIGITAL 6

// OLED 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// OBJECTS 
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
DHT dht(DHTPIN, DHTTYPE);

// GLOBALS 
String nmeaLine = "";

unsigned long lastLog = 0;
const unsigned long logInterval = 2000;

float temperature = 0;
float humidity = 0;
int rainA = 0;
int rainD = 0;

int snrValues[15];
int snrCount = 0;

int maxSNR = 0;
String rainLevel = "None";

// SETUP 
void setup() {

  Serial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();

  pinMode(RAIN_DIGITAL, INPUT);

  // OLED init 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED failed");
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("NimbusAI System");
  display.display();
  delay(2000);

  // SD card 
  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed");
    while (1);
  }

  if (!SD.exists("data.csv")) {
    File f = SD.open("data.csv", FILE_WRITE);

    f.print("Time,Temp,Humidity,RainA,RainD");

    for (int i=1;i<=15;i++){
      f.print(",SNR");
      f.print(i);
    }

    f.println(",MaxSNR,RainLevel");

    f.close();
  }

  Serial.println("System Ready");
}

// LOOP 
void loop() {

  while (gpsSerial.available()) {

    char c = gpsSerial.read();

    if (c == '\n') {
      parseGPGSV(nmeaLine);
      nmeaLine = "";
    }

    else if (c != '\r') {
      nmeaLine += c;
    }
  }

  if (millis() - lastLog >= logInterval) {

    lastLog = millis();

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    rainA = analogRead(RAIN_ANALOG);
    rainD = digitalRead(RAIN_DIGITAL);

    calculateRainLevel();

    displayData();

    writeRow();

    clearSNR();
  }
}

// GPGSV PARSER 

void parseGPGSV(String line) {

  if (!line.startsWith("$GPGSV")) return;

  char buffer[150];
  line.toCharArray(buffer,sizeof(buffer));

  char *token = strtok(buffer,",");
  int field = 0;

  while(token != NULL){

    field++;

    if(field>=5 && (field-5)%4==0){

      strtok(NULL,",");
      strtok(NULL,",");
      token = strtok(NULL,",");

      int snr = token ? atoi(token) : 0;

      if(snr>0 && snr<=60 && snrCount<15){
        snrValues[snrCount++] = snr;
      }
    }

    token = strtok(NULL,",");
  }
}

// MAX SNR + RAIN LEVEL 

void calculateRainLevel(){

  maxSNR = 0;

  for(int i=0;i<snrCount;i++){

    if(snrValues[i] > maxSNR){
      maxSNR = snrValues[i];
    }
  }

  if(maxSNR >= 40 && maxSNR <= 60)
    rainLevel = "No Rain";

  else if(maxSNR >=30 && maxSNR <40)
    rainLevel = "Low Rain";

  else if(maxSNR <30 && maxSNR>0)
    rainLevel = "High Rain";

  else
    rainLevel = "No Data";
}

// OLED DISPLAY 

void displayData(){

  display.clearDisplay();

  display.setCursor(0,0);
  display.print("Tem:");
  display.print(temperature);
  display.print("C ");

  display.print("Humidity:");
  display.print(humidity);
  display.println("%");

  display.setCursor(0,12);
  display.print("MaxSNR:");
  display.println(maxSNR);

  display.setCursor(0,22);
  display.print("Rain:");
  display.println(rainLevel);

  display.display();
}

// WRITE CSV 

void writeRow(){

  File f = SD.open("data.csv", FILE_WRITE);

  if(f){

    f.print(millis());
    f.print(",");
    f.print(temperature);
    f.print(",");
    f.print(humidity);
    f.print(",");
    f.print(rainA);
    f.print(",");
    f.print(rainD);

    for(int i=0;i<15;i++){
      f.print(",");
      f.print(snrValues[i]);
    }

    f.print(",");
    f.print(maxSNR);
    f.print(",");
    f.println(rainLevel);

    f.close();
  }
}

// CLEAR SNR 

void clearSNR(){

  for(int i=0;i<15;i++){
    snrValues[i] = 0;
  }

  snrCount = 0;
}