#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Create TFT instance

#define OUT1 16 // Sensor 1 output pin
#define OUT2 17 // Sensor 2 output pin

#define BLACK_THRESHOLD 15000  // Adjust as needed
#define TIMEOUT_PULSE 30000    // 30 ms timeout for pulseIn

unsigned long pulseDuration1, pulseDuration2;
unsigned int frequency1, frequency2;

void displayMessage(const char* label, unsigned int freq, const char* msg, int y, uint16_t bgColor, uint16_t textColor) {
  // Draw a message for one sensor at position Y
  tft.fillRect(0, y - 10, 240, 40, bgColor);
  tft.setTextColor(textColor, bgColor);
  tft.setCursor(10, y);
  tft.printf("%s: %s (%u Hz)", label, msg, freq);
}

void setup() {
  Serial.begin(115200);

  pinMode(OUT1, INPUT);
  pinMode(OUT2, INPUT);

  // Initialize TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  Serial.println("Dual sensor color detector ready.");
}

void loop() {
  // --- Read both sensors ---
  pulseDuration1 = pulseIn(OUT1, LOW, TIMEOUT_PULSE);
  pulseDuration2 = pulseIn(OUT2, LOW, TIMEOUT_PULSE);

  if (pulseDuration1 == 0) pulseDuration1 = TIMEOUT_PULSE;
  if (pulseDuration2 == 0) pulseDuration2 = TIMEOUT_PULSE;

  frequency1 = 1000000UL / pulseDuration1;
  frequency2 = 1000000UL / pulseDuration2;

  // --- Print to serial ---
  Serial.print("Sensor 1 Frequency: ");
  Serial.println(frequency1);
  Serial.print("Sensor 2 Frequency: ");
  Serial.println(frequency2);

  // --- Determine colors ---
  const char* msg1;
  const char* msg2;
  uint16_t bg1, fg1, bg2, fg2;

  if (frequency1 > 100000) {
    msg1 = "No valid";
    bg1 = TFT_YELLOW; fg1 = TFT_BLACK;
  } else if (frequency1 < BLACK_THRESHOLD) {
    msg1 = "BLACK";
    bg1 = TFT_BLACK; fg1 = TFT_WHITE;
  } else {
    msg1 = "WHITE";
    bg1 = TFT_WHITE; fg1 = TFT_BLACK;
  }

  if (frequency2 > 100000) {
    msg2 = "No valid";
    bg2 = TFT_YELLOW; fg2 = TFT_BLACK;
  } else if (frequency2 < BLACK_THRESHOLD) {
    msg2 = "BLACK";
    bg2 = TFT_BLACK; fg2 = TFT_WHITE;
  } else {
    msg2 = "WHITE";
    bg2 = TFT_WHITE; fg2 = TFT_BLACK;
  }

  // --- Display both results on screen ---
  displayMessage("S1", frequency1, msg1, 40, bg1, fg1);
  displayMessage("S2", frequency2, msg2, 100, bg2, fg2);

  delay(300);
}
