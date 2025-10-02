/**
Note intended to only work with single TTGO and single Sensor
- later a version of code with TWO sensors will be developed
  - Just requires 2 pins and some OR logic

Sensors obivously require ground and vcc (5V) connection too
*/

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Create TFT instance

#define OUT 16 // Only 1 pin needed (sensor output)
//Later this will be shared by both? OR gate it somehow 
//or just have two output pins

// Calibration values

//THRESHOLD FOR BLACK AND WHITE - INCREASE IF REQUIRED
//Doen't work if too far away 
#define BLACK_THRESHOLD 18000  

#define TIMEOUT_PULSE 30000      // Increase timeout to 30 ms for pulseIn

unsigned long pulseDuration;

unsigned int frequency;

void displayMessage(const char* msg, uint16_t bgColor, uint16_t textColor) {
  tft.fillScreen(bgColor);
  tft.setTextColor(textColor, bgColor);
  tft.setCursor(10, 50);
  tft.println(msg);
}

void setup() {
  Serial.begin(115200);

  pinMode(OUT, INPUT); // Sensor output pin

  // Initialize TFT screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void loop() {
  pulseDuration = pulseIn(OUT, LOW, TIMEOUT_PULSE);

  // Handle timeout if no pulse is detected
  if (pulseDuration == 0) {
    pulseDuration = TIMEOUT_PULSE;
  }

  frequency = 1000000UL / pulseDuration;

  Serial.print("Frequency: ");
  Serial.println(frequency);

  if (frequency > 100000) {
    Serial.println("No valid reading");
    displayMessage("probably white", TFT_YELLOW, TFT_BLACK);
  } else if (frequency < BLACK_THRESHOLD) {
    Serial.println("Black detected");
    displayMessage("BLACK", TFT_BLACK, TFT_WHITE);
  } else {
    Serial.println("White detected");
    displayMessage("WHITE", TFT_WHITE, TFT_BLACK);
  }

  delay(300); // Delay to avoid flicker and allow stable reading
}
