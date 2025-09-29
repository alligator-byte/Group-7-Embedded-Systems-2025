/** WORK OF GROUP 7
 * Following code works with TTGO to and QTR SENSOR to:
 * - spot the colour white (reflective)
 * - spot the colour black (non-reflective)
 * 
 * This will be used in combination with rest of robot code
 */


#include <Arduino.h>
#include <TFT_eSPI.h>
 
TFT_eSPI tft = TFT_eSPI();
 
// Sensor pin
#define QTR_SENSOR_PIN 4  // Change if using another pin

void setup() {
  // Start Serial and TFT
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Set sensor pin as input
  pinMode(QTR_SENSOR_PIN, INPUT);
}


void loop() {
  int sensorValue = digitalRead(QTR_SENSOR_PIN); //Ask "what am looking at?"

  if (sensorValue == HIGH) {
    // Reflective surface (white)
    Serial.println("White detected");
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setCursor(10, 50);
    tft.println("WHITE");
  } else {
    // Non-reflective surface (black)
    Serial.println("Black detected");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(10, 50);
    tft.println("BLACK");
  }

  delay(200); // Reduce flicker
}

