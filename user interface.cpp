void updateDisplay(float stressScore) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Stress: ");
  display.println(stressScore);
  display.print("Temp: ");
  display.println(temp);
  // Add emoji for "emotion" 😊 🥵
  display.display();
}