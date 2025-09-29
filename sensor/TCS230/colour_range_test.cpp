#include <Arduino.h>
#include <TFT_eSPI.h>
 
TFT_eSPI tft = TFT_eSPI();
 
#define S0 10
#define S1 11
#define S2 12
#define S3 13
#define OUT 16
 
unsigned long pulseDuration;
unsigned int frequency;
 
void setFilter(int filter) {
  switch(filter) {
    case 0: // Clear
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
    case 1: // Red
      digitalWrite(S2, LOW);
      digitalWrite(S3, HIGH);
      break;
    case 2: // Green
      digitalWrite(S2, HIGH);
      digitalWrite(S3, HIGH);
      break;
    case 3: // Blue
      digitalWrite(S2, HIGH);
      digitalWrite(S3, LOW);
      break;
  }
}
 
void measureAndPrint(const char* color) {
  pulseDuration = pulseIn(OUT, LOW, 20000); // 20ms timeout
  if (pulseDuration == 0) pulseDuration = 20000; // timeout = max pulse length
 
  frequency = 1000000UL / pulseDuration;
 
  Serial.print(color);
  Serial.print(": ");
  Serial.println(frequency);
}
 
 
void setup() {
  Serial.begin(115200);
 
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
 
  // Set frequency scaling to 100% for max signal
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
}
 
void loop() {
  // Cycle through filters
  setFilter(0); // Clear
  measureAndPrint("Clear");
 
  setFilter(1); // Red
  measureAndPrint("Red");
 
  setFilter(2); // Green
  measureAndPrint("Green");
 
  setFilter(3); // Blue
  measureAndPrint("Blue");
 
  Serial.println();
 
  delay(1000);
}
 
