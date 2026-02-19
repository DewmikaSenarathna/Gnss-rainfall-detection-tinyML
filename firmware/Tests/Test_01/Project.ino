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

#define RAIN_PIN PA0
#define BUZZER_PIN PA2

// SNR Storage 
float snr_values[15];

// Function Prototype 
void read_snr_from_gps(float snr_array[15]);

//                     SETUP

void setup()
{
    Serial.begin(9600);   // GPS UART

    dht.begin();
    pinMode(RAIN_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

//                     LOOP

void loop()
{
    // Read SNR from GPS
    read_snr_from_gps(snr_values);

    // Predict Rain using Neural Network
    int result = predict_rain(snr_values);

    // Read Other Sensors
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int rain_adc = analogRead(RAIN_PIN);

    // Display Data
    display.clearDisplay();
    display.setCursor(0,0);

    display.print("Temp: ");
    display.println(temperature);

    display.print("Hum: ");
    display.println(humidity);

    display.print("Rain ADC: ");
    display.println(rain_adc);

    display.print("Status: ");

    if(result == 0)
        display.println("Sunny");
    else if(result == 1)
        display.println("Low Rain");
    else
        display.println("High Rain");

    display.display();

    // Buzzer Control
    if(result == 2)
        digitalWrite(BUZZER_PIN, HIGH);
    else
        digitalWrite(BUZZER_PIN, LOW);

    delay(3000);
}

// GPS SNR READING FUNCTION

void read_snr_from_gps(float snr_array[15])
{
    // Reset SNR array
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

                    // SNR fields occur every 4th field after satellite info
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
