/*
This is the first prototype featuring both sensor and movement of the sumo robot
*/

#include <Arduino.h>
#include <TFT_eSPI.h>

// --- Stay in the RING!!!!
#define F_FLOOR 12 //COLOUR SENSOR FRONT
#define B_FLOOR 13 //COLOUR SENSOR BACK
#define BLACK_THRESHOLD 15000
#define TIMEOUT_PULSE 30000


//connected to motor driver in
const int sig1 = 1;
const int sig2 = 2;
const int sig3 = 43;
const int sig4 = 44;

//ultrasonic sensor pins
const int trigPin = 18;
const int echoPin = 17;
const int POWER = 15;

//colour sensor tracking
unsigned long pulseDuration1, pulseDuration2;
unsigned int frequency1, frequency2;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
float distanceCm;
float lastDistance = -1;   


TFT_eSPI tft = TFT_eSPI(); //invokes libs

//motor movement fns
void stopAllMotors() {
  
  digitalWrite(sig1, LOW);
  digitalWrite(sig2, LOW);
  digitalWrite(sig3, LOW);
  digitalWrite(sig4, LOW);
}

void driveForwardFull() {
  // Motor A full forward
  digitalWrite(sig1, HIGH);
  digitalWrite(sig2, LOW);
  // Motor B full forward
  digitalWrite(sig3, HIGH);
  digitalWrite(sig4, LOW);
}

void turn_left() {
  // Motor A full backward
  digitalWrite(sig1, LOW);
  digitalWrite(sig2, HIGH);
  // Motor B full forward
  digitalWrite(sig3, HIGH);
  digitalWrite(sig4, LOW);
  }

  void turn_right() {
  // Motor A full forward
  digitalWrite(sig1, HIGH);
  digitalWrite(sig2, LOW);
  // Motor B full backward
  digitalWrite(sig3, LOW);
  digitalWrite(sig4, HIGH);
  }

// update display fn
void updateDisplay() {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.printf("Opponent Dist: %.1f cm   ", distanceCm); // extra spaces overwrite old values

  //colour ---> CHECK THIS WORKS******************************************************************************************************
  tft.setCursor(10, 50);
  tft.printf("L:%u  R:%u    ", frequency1, frequency2);
}

/* FOR COLOUR SENSOR
*
* Takes in Boolean pair and assigns them values based on the two sensors
* If required - the sensors INPUT pins may be changed
*/
void readLightSensors(bool &BackBlack, bool &FrontBlack) {
  // Measure pulse width for both sensors
  pulseDuration1 = pulseIn(F_FLOOR, LOW, TIMEOUT_PULSE);
  pulseDuration2 = pulseIn(B_FLOOR, LOW, TIMEOUT_PULSE);

  if (pulseDuration1 == 0) pulseDuration1 = TIMEOUT_PULSE;
  if (pulseDuration2 == 0) pulseDuration2 = TIMEOUT_PULSE;

  frequency1 = 1000000UL / pulseDuration1;
  frequency2 = 1000000UL / pulseDuration2;

  Serial.printf("L: %u Hz | R: %u Hz\n", frequency1, frequency2);

  // Determine black/white state
  FrontBlack = (frequency1 < BLACK_THRESHOLD);
  BackBlack = (frequency2 < BLACK_THRESHOLD);
}


//--setup stuff---
void setup() {
  // Motor setup stuff 
  digitalWrite(POWER, HIGH); //turn on power to ultrasonic sensor
  pinMode(sig1, OUTPUT);
  pinMode(sig2, OUTPUT); 
  pinMode(sig3, OUTPUT);
  pinMode(sig4, OUTPUT);
  pinMode(POWER, OUTPUT);
  stopAllMotors();

  //Colour floor
  pinMode(F_FLOOR, INPUT);
  pinMode(B_FLOOR, INPUT);

  // Ultrasonic setup stuff
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);  
  updateDisplay();
}

//-----Motor control stuff-----
void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;

  //default directive 
  turn_right();


  // Update display only if distance changed drastically
  if (abs(distanceCm - lastDistance) > 0.5) {
    updateDisplay();
    lastDistance = distanceCm;
  }

  //If closer than 30cm → drive forward
  if (distanceCm > 0 && distanceCm < 30.0) {
    driveForwardFull();
  } else {
    stopAllMotors();
  }

  bool BackBlack, FrontBlack;
  readLightSensors(BackBlack, FrontBlack);

  // Example logic: line following
  if (BackBlack) {
    //TODO: Handle
  } 
  else if (FrontBlack) {
    //TODO: Handle
  } 
  else {
    // Both white ---> continue normal operation
    driveForwardFull();
  }

  delay(100); 
}
