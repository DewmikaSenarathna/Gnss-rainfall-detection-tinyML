#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#include "rf_model.h"
#include "gnss_filter.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define DHTPIN PA1
#define DHTTYPE DHT11

HardwareSerial GNSS(USART1);
DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Satellite sats[20];
int sat_count = 0;

String nmea_buffer = "";

float temperature = 0;
float humidity = 0;
float max_snr = 0;

void parseGPGSV(String line);
void read_gnss();
void show_display(int prediction);

void setup()
{
  Serial.begin(115200);
  GNSS.begin(9600);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  dht.begin();
}

void loop()
{
  read_gnss();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  max_snr = get_max_snr(sats, sat_count);

  double features[7];
  double result[3];

  features[0] = temperature;
  features[1] = humidity;
  features[2] = sats[0].azimuth;
  features[3] = sats[0].elevation;
  features[4] = max_snr;
  features[5] = sats[0].prn;
  features[6] = sat_count;

  score(features, result);

  int prediction = 0;

  if(result[1] > result[prediction]) prediction = 1;
  if(result[2] > result[prediction]) prediction = 2;

  show_display(prediction);

  delay(2000);
}

void read_gnss()
{
  while(GNSS.available())
  {
    char c = GNSS.read();

    if(c=='\n')
    {
      if(nmea_buffer.startsWith("$GPGSV"))
      {
        parseGPGSV(nmea_buffer);
      }

      nmea_buffer="";
    }
    else
    {
      nmea_buffer += c;
    }
  }
}

void parseGPGSV(String line)
{
  sat_count = 0;

  int field = 0;
  String token = "";

  for(int i=0;i<line.length();i++)
  {
    if(line[i]==',' || line[i]=='*')
    {
      field++;

      if(field>=4)
      {
        int mod = (field-4)%4;

        if(mod==0)
          sats[sat_count].prn = token.toInt();

        if(mod==1)
          sats[sat_count].elevation = token.toFloat();

        if(mod==2)
          sats[sat_count].azimuth = token.toFloat();

        if(mod==3)
        {
          sats[sat_count].snr = token.toFloat();
          sat_count++;
        }
      }

      token="";
    }
    else
    {
      token += line[i];
    }
  }
}

void show_display(int prediction)
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("T:");
  display.print(temperature);

  display.setCursor(64,0);
  display.print("H:");
  display.print(humidity);

  display.setCursor(0,12);
  display.print("SNR:");
  display.print(max_snr);

  display.setCursor(0,22);

  if(prediction==0)
    display.print("SUNNY");

  if(prediction==1)
    display.print("LOW RAIN");

  if(prediction==2)
    display.print("HEAVY RAIN");

  display.display();
}