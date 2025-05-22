#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_PIN A0
#define RELAY_PIN 8
#define LED_PIN 10
#define BUZZER_PIN 11
#define MODE_PIN 7  // Auto/Manual Switch

int soilMoisture;
float temperature;
float humidity;
bool autoMode;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT_PULLUP);  // Switch HIGH = Manual, LOW = Auto

  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  autoMode = digitalRead(MODE_PIN) == LOW;

  // Read sensors
  soilMoisture = analogRead(SOIL_PIN);
  int soilPercent = map(soilMoisture, 1023, 0, 0, 100);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // LED Sunlight Simulation
  if (soilPercent < 50) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Auto watering
  if (autoMode) {
    if (soilPercent < 40) {
      digitalWrite(RELAY_PIN, HIGH);  // Turn pump ON
    } else if (soilPercent > 60) {
      digitalWrite(RELAY_PIN, LOW);   // Turn pump OFF
    }
  }

  // Buzzer alerts
  if (temperature > 35 || humidity < 30) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.print("Soil: "); display.print(soilPercent); display.println(" %");
  display.print("Mode: "); display.println(autoMode ? "AUTO" : "MANUAL");
  display.display();

  delay(2000);
}
