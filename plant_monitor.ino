#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_PIN A0
#define RELAY_PIN 8
#define LED_PIN 10
#define BUZZER_PIN 11
#define MODE_PIN 7  // Auto(LOW)/Manual(HIGH)

#define SOIL_DRY 600  // Analog value when dry (0% moisture)
#define SOIL_WET 300  // Analog value when wet (100% moisture)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

int soilMoistRaw;
int soilPercent;
float temperature;
float humidity;
bool autoMode;
bool relayState = false;
unsigned long lastBeepTime = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT_PULLUP);  // Internal pull-up

  dht.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed!");
    while (true);  // Halt if display fails
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  autoMode = digitalRead(MODE_PIN) == LOW;

  soilMoistRaw = analogRead(SOIL_PIN);
  soilPercent = constrain(map(soilMoistRaw, SOIL_DRY, SOIL_WET, 0, 100), 0, 100);
  
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT read error!");
    displayError("DHT Error");
    delay(1000);
    return;  // Skip this loop iteration
  }

  digitalWrite(LED_PIN, soilPercent < 50 ? HIGH : LOW);

  if (autoMode) {
    if (soilPercent < 40 && !relayState) {
      digitalWrite(RELAY_PIN, HIGH);
      relayState = true;
    } 
    else if (soilPercent > 60 && relayState) {
      digitalWrite(RELAY_PIN, LOW);
      relayState = false;
    }
  }

  if (temperature > 35 || humidity < 30) {
    if (millis() - lastBeepTime > 1000) {  // Beep every 1s
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
      lastBeepTime = millis();
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  updateDisplay();
  delay(1000);  // Reduced from 2000ms for better responsiveness
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  
  display.print("Temp: "); display.print(temperature, 1); display.println(" C");
  display.print("Humid: "); display.print(humidity, 1); display.println(" %");
  display.print("Soil: "); display.print(soilPercent); display.println(" %");
  display.print("Mode: "); display.println(autoMode ? "AUTO" : "MANUAL");
  display.print("Pump: "); display.println(relayState ? "ON" : "OFF");
  
  display.display();
}

void displayError(const char* msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("ERROR:");
  display.println(msg);
  display.display();
}
