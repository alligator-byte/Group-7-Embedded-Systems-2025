/** (5th PROTOTYPE)
 * CHANGES MADE:
 * - Uses a threshold of 25000; reads floor sensors at the start of the loop; 
 * - Higher accuracy to the Ring that will be used in the final competition 
 * - adds an action/status line to the display and prints sensor states.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

// --- Stay in the RING!!!!
#define F_FLOOR 12 //COLOUR SENSOR FRONT
#define B_FLOOR 13 //COLOUR SENSOR BACK
#define BLACK_THRESHOLD 25000
#define TIMEOUT_PULSE 30000

//connected to motor driver in
const int sig1 = 1;
const int sig2 = 2;
const int sig3 = 43;
const int sig4 = 44;

//ultrasonic sensor pins
const int trigPin = 18; //FRONT
const int echoPin = 17;
const int POWER = 15;

unsigned long lastDetectedTime = 0; //"are you still there?" -like from portal 
const unsigned long SEARCH_DELAY = 2000; // 2 s before search mode

//colour sensor tracking
unsigned long pulseDuration1, pulseDuration2;
unsigned int frequency1, frequency2;
bool BackBlack, FrontBlack; //bool vals; TRUE WHEN BLACK

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

void driveBackwardFull() {
  // Motor A backward
  digitalWrite(sig1, LOW);
  digitalWrite(sig2, HIGH);
  // Motor B backward
  digitalWrite(sig3, LOW);
  digitalWrite(sig4, HIGH);
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


//Remove blocking delays**********************************************NEW
unsigned long actionStartTime = 0;
unsigned long actionDuration = 0; //how long operation goes for
enum Action { NONE, FORWARD, BACKWARD, TURN_RIGHT };
Action currentAction = NONE; //give enums value


//ignore - just for printing
const char* actionToString(Action action) {
  switch (action) {
    case FORWARD: return "FORWARD";
    case BACKWARD: return "BACKWARD";
    case TURN_RIGHT: return "TURN_RIGHT";
    case NONE: default: return "IDLE";
  }
}


// update display fn
void updateDisplay() {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.printf("Opponent Dist: %.1f cm   ", distanceCm); // extra spaces overwrite old values

  tft.setCursor(10, 50); //show colour
  tft.printf("F:%u %s  B:%u %s   ",
    frequency1, FrontBlack ? "B" : "W",
    frequency2, BackBlack ? "B" : "W");

  tft.setCursor(10, 80); //show action invoked
  tft.printf("Status: %s    ", actionToString(currentAction));
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

  Serial.printf("Front :%u %s  Bback:%u %s   ",
    frequency1, FrontBlack ? "BLACK" : "WHITE",
    frequency2, BackBlack ? "BLACK" : "WHITE");

  // Determine black/white state
  FrontBlack = (frequency1 < BLACK_THRESHOLD);
  BackBlack = (frequency2 < BLACK_THRESHOLD);

  updateDisplay();
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
  //check colour first
  readLightSensors(BackBlack, FrontBlack);
  
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;

  // Check if an opponent is detected
  if (distanceCm > 0 && distanceCm < 30.0) {
    lastDetectedTime = millis(); // reset timer when opponent seen
  }

  /*default directive: 
  turn_right();

  relocated to end of ELSE of this loop()
  */


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

  unsigned long now = millis();

  if (currentAction == NONE){
    if(BackBlack){ //Black detect @ behind >>> drive forward
      driveForwardFull();
      currentAction = FORWARD;
      actionStartTime = now;
      actionDuration = 300; //ms, adjust when req.

    } else if (FrontBlack) { //Black @ front >>> Drive backward
      driveBackwardFull();
      currentAction = BACKWARD;
      actionStartTime = now;
      actionDuration = 300;
    }

    else if (now - lastDetectedTime > SEARCH_DELAY){
      turn_right();
      currentAction = TURN_RIGHT;
      actionDuration = 200;
      actionStartTime = now;
    }
    else if (distanceCm > 0 && distanceCm <30.0){
      driveForwardFull();
    } else {
      stopAllMotors(); //halts when doing nothing
      currentAction = NONE; //allows it to check
    }

    }

  // Handle action timing (non-blocking)
  if (currentAction != NONE && (now - actionStartTime >= actionDuration)) {
    stopAllMotors();
    currentAction = NONE;
  }

  }
