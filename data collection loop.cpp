void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(A0);
  float lux = lightMeter.readLightLevel();
  int edaValue = analogRead(A1); // "Plant stress"

  logData(temp, humidity, soilMoisture, lux, edaValue);
  delay(1000);
}