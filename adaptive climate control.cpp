void controlActuators(float stressScore) {
  // Water pump (prevent over/under-watering)
  if (soilMoisture < 30 && stressScore > 60) {
    digitalWrite(RELAY_PIN, HIGH);
  }

  // Grow light (dim if "stressed")
  analogWrite(GROW_LIGHT_PIN, stressScore > 70 ? 128 : 255);

  // Ventilation (cool if stressed)
  if (temp > 28 || stressScore > 75) {
    servo.write(90); // Open vent
  }
}