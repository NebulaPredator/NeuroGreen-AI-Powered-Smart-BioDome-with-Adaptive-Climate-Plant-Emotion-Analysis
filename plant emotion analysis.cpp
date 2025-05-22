float calculateStressScore(int eda, float lux, int soil) {
  float stress = 0;
  stress += map(eda, 0, 1023, 0, 50);          // EDA contributes 0-50%
  stress += (lux < 5000) ? 30 : 0;              // Low light = +30%
  stress += (soil < 30) ? 20 : 0;               // Dry soil = +20%
  return constrain(stress, 0, 100);
}