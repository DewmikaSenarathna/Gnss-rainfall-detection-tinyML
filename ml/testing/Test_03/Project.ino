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
float get_max_snr(float snr_array[15]);

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

    // Get highest SNR
    float max_snr = get_max_snr(snr_values);

    // Predict Rain using Neural Network
    int result = predict_rain(snr_values);

    // Read Temperature & Humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Display on OLED
    display.clearDisplay();
    display.setCursor(0,0);

    display.print("T:");
    display.print(temperature,1);
    display.print("C ");

    display.print("H:");
    display.print(humidity,1);
    display.println("%");

    display.print("Max SNR: ");
    display.println(max_snr,1);

    display.print("Status: ");

    if(result == 0)
        display.println("Sunny");
    else if(result == 1)
        display.println("High");
    else
        display.println("Low");

    display.display();

    // Buzzer
    digitalWrite(BUZZER_PIN, (result == 1) ? HIGH : LOW);

    delay(3000);
}


float get_max_snr(float snr_array[15])
{
    float max_val = snr_array[0];

    for(int i = 1; i < 15; i++)
    {
        if(snr_array[i] > max_val)
            max_val = snr_array[i];
    }

    return max_val;
}


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
