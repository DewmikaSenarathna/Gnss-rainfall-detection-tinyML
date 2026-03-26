#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>

/* ---------- PINS ---------- */
#define GPS_RX 4
#define GPS_TX 3
#define SD_CS 10

#define DHTPIN 5
#define DHTTYPE DHT11

#define RAIN_ANALOG A0
#define RAIN_DIGITAL 6

/* ---------- OBJECTS ---------- */
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
DHT dht(DHTPIN, DHTTYPE);

/* ---------- GLOBALS ---------- */
String nmeaLine = "";

unsigned long lastLog = 0;
const unsigned long logInterval = 2000;

float temperature = 0;
float humidity = 0;
int rainA = 0;
int rainD = 0;

int snrValues[15];   // store up to 15 satellite SNRs
int snrCount = 0;

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();

  pinMode(RAIN_DIGITAL, INPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed");
    while (1);
  }

  if (!SD.exists("data.csv")) {
    File f = SD.open("data.csv", FILE_WRITE);
    f.print("Time(ms),Temp(C),Humidity(%),RainAnalog,RainDigital");
    for (int i = 1; i <= 15; i++) {
      f.print(",SNR");
      f.print(i);
    }
    f.println();
    f.close();
  }

  Serial.println("System Ready");
}

/* ---------- LOOP ---------- */
void loop() {

  /* ---- Read GPS continuously ---- */
  while (gpsSerial.available()) {
    char c = gpsSerial.read();

    if (c == '\n') {
      parseGPGSV(nmeaLine);
      nmeaLine = "";
    } else if (c != '\r') {
      nmeaLine += c;
    }
  }

  /* ---- Log every 2 seconds ---- */
  if (millis() - lastLog >= logInterval) {
    lastLog = millis();

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    rainA = analogRead(RAIN_ANALOG);
    rainD = digitalRead(RAIN_DIGITAL);

    writeRow();
    clearSNR();
  }
}

/* ---------- GPGSV PARSER ---------- */
void parseGPGSV(String line) {

  if (!line.startsWith("$GPGSV")) return;

  char buffer[150];
  line.toCharArray(buffer, sizeof(buffer));

  char *token = strtok(buffer, ",");
  int field = 0;

  while (token != NULL) {
    field++;

    // Satellite blocks start at field 5
    if (field >= 5 && (field - 5) % 4 == 0) {
      int satID = atoi(token);          // PRN
      strtok(NULL, ",");                // elevation
      strtok(NULL, ",");                // azimuth
      token = strtok(NULL, ",");        // SNR

      int snr = token ? atoi(token) : 0;

      if (snr > 0 && snrCount < 15) {
        snrValues[snrCount++] = snr;
      }
    }

    token = strtok(NULL, ",");
  }
}

/* ---------- WRITE CSV ROW ---------- */
void writeRow() {

  /* ---- SERIAL ---- */
  Serial.print("Time: ");
  Serial.print(millis());
  Serial.print(" | T: ");
  Serial.print(temperature);
  Serial.print(" C | H: ");
  Serial.print(humidity);
  Serial.print(" % | RainA: ");
  Serial.print(rainA);
  Serial.print(" | RainD: ");
  Serial.print(rainD);
  Serial.print(" | SNRs: ");

  for (int i = 0; i < 15; i++) {
    Serial.print(snrValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  /* ---- SD ---- */
  File f = SD.open("data.csv", FILE_WRITE);
  if (f) {
    f.print(millis());
    f.print(",");
    f.print(temperature);
    f.print(",");
    f.print(humidity);
    f.print(",");
    f.print(rainA);
    f.print(",");
    f.print(rainD);

    for (int i = 0; i < 15; i++) {
      f.print(",");
      f.print(snrValues[i]);
    }
    f.println();
    f.close();
  }
}

/* ---------- CLEAR SNR BUFFER ---------- */
void clearSNR() {
  for (int i = 0; i < 15; i++) {
    snrValues[i] = 0;
  }
  snrCount = 0;
}
