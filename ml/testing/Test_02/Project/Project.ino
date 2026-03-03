#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#include "neural_network.h"

// OLED Configuration 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensor Configuration
#define DHTPIN PA1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BUZZER_PIN PA2

// SNR Storage 
float snr_values[15];

// Function Prototype 
void read_snr_from_gps(float snr_array[15]);

// SETUP 
void setup()
{
    Serial.begin(9600);   // GPS UART

    dht.begin();
    pinMode(BUZZER_PIN, OUTPUT);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

// LOOP
void loop()
{
    // Read SNR from GPS
    read_snr_from_gps(snr_values);

    // Predict Rain
    int result = predict_rain(snr_values);

    // Read Temperature & Humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Display on OLED
    display.clearDisplay();
    display.setCursor(0,0);

    // First Line: Temperature & Humidity
    display.print("T:");
    display.print(temperature,1);
    display.print("C ");

    display.print("H:");
    display.print(humidity,1);
    display.println("%");

    // Second Line: First 2 SNR values
    display.print("S1:");
    display.print(snr_values[0],1);
    display.print(" S2:");
    display.println(snr_values[1],1);

    // Third Line: Third SNR value
    display.print("S3:");
    display.println(snr_values[2],1);

    // Fourth Line: Prediction Status
    display.print("Status:");

    if(result == 0)
        display.println("Sunny");
    else if(result == 1)
        display.println("Low");
    else
        display.println("High");

    display.display();

    // 5️⃣ Buzzer Control
    digitalWrite(BUZZER_PIN, (result == 2) ? HIGH : LOW);

    delay(3000);
}

// GPS SNR FUNCTION 
void read_snr_from_gps(float snr_array[15])
{
    for(int i = 0; i < 15; i++)
        snr_array[i] = 0;

    int index = 0;

    while(Serial.available() > 0)
    {
        String line = Serial.readStringUntil('\n');

        if(line.startsWith("$GPGSV"))
        {
            int field = 0;

            for(int i = 0; i < line.length(); i++)
            {
                if(line[i] == ',')
                {
                    field++;

                    if((field - 4) % 4 == 0 && field > 4)
                    {
                        int end = line.indexOf(',', i+1);
                        if(end == -1)
                            end = line.length();

                        String snr_str = line.substring(i+1, end);
                        snr_array[index++] = snr_str.toFloat();

                        if(index >= 15)
                            return;
                    }
                }
            }
        }
    }
}
