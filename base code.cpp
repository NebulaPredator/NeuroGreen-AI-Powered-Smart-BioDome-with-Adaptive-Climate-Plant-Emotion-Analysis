#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BH1750.h>

// Sensors
DHT dht(D2, DHT22);
BH1750 lightMeter;
Adafruit_SSD1306 display(128, 64, &Wire);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lightMeter.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}