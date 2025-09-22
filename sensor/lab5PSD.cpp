#include <Arduino.h>
#include <TFT_eSPI.h>
 
#define SERVO_PIN 2     // PWM pin for servo
#define TX_PIN 17
#define RX_PIN 18
 
TFT_eSPI tft = TFT_eSPI();
 
// LEDC config
const int SERVO_CHANNEL = 0;
const int SERVO_FREQ = 50;   // 50 Hz
const int SERVO_RES = 16;    // 16-bit
const int DUTY_MAX = (1 << SERVO_RES) - 1;
 
const float minUptime = 1.0;   // ms
const float maxUptime = 2.0;   // ms
const float periodMs  = 20.0;  // 50 Hz
 
// Distance (mm) vs ADC table (descending ADC)
const float distanceTable[] = {50, 100, 150, 200, 250, 300};
const float voltageTable[]  = {2079,1267, 773, 687, 501, 440};
const int NUM_POINTS = 6;
 
static inline float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
 
float adcToDistance(int adc) {
  // Clamp outside the table range
  if (adc >= voltageTable[0]) return distanceTable[0];           // closer than 50 mm
  if (adc <= voltageTable[NUM_POINTS-1]) return distanceTable[NUM_POINTS-1]; // farther than 300 mm
 
  for (int i = 0; i < NUM_POINTS - 1; i++) {
    float v1 = voltageTable[i];
    float v2 = voltageTable[i+1];
    if (adc <= v1 && adc >= v2) {
      float d1 = distanceTable[i];
      float d2 = distanceTable[i+1];
      return d1 + (d2 - d1) * (v1 - adc) / (v1 - v2); // linear interpolation
    }
  }
  return distanceTable[NUM_POINTS-1];
}
 
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
 
  ledcSetup(SERVO_CHANNEL, SERVO_FREQ, SERVO_RES);
  ledcAttachPin(SERVO_PIN, SERVO_CHANNEL);
 
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println("Receiver ready");
  tft.println("Waiting for data...");
}
 
void loop() {
  if (Serial2.available() >= 2) {
    byte highByte = Serial2.read();
    byte lowByte  = Serial2.read();
    int adcValue = (highByte << 8) | lowByte;
 
    float distance = adcToDistance(adcValue);
    float uptimeMs = mapf(distance, 50.0f, 300.0f, minUptime, maxUptime);
    if (uptimeMs < minUptime) uptimeMs = minUptime;
    if (uptimeMs > maxUptime) uptimeMs = maxUptime;
 
    int pulseWidthUs = (int)(uptimeMs * 1000.0f);
    int duty = (int)((pulseWidthUs / (periodMs * 1000.0f)) * DUTY_MAX);
    if (duty < 0) duty = 0;
    if (duty > DUTY_MAX) duty = DUTY_MAX;
    ledcWrite(SERVO_CHANNEL, duty);
 
    // Display
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 20);
    tft.printf("ADC: %d\n", adcValue);
    tft.printf("Dist: %.1f mm\n", distance);
    tft.printf("Pulse: %dus\n", pulseWidthUs);
 
    // Debug
    Serial.printf("ADC=%d, Dist=%.1fmm, PW=%dus, duty=%d\n", adcValue, distance, pulseWidthUs, duty);
  }
}
