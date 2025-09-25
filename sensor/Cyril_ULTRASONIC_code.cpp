#include <Arduino.h>
#include <TFT_eSPI.h>

const int trigPin = 43;
const int echoPin = 44;
const int led = 2;
const int buzzer = 1;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;


TFT_eSPI tft =  TFT_eSPI();
void updateDisplay() {  //sets up text display stuff
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.printf("OPPONENT PROXIMITY (Cm): %.1f cm", distanceCm);

}




void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Display
  tft.init();
  tft.setRotation(1);
  updateDisplay();
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  updateDisplay();
  
  // Activate LED and buzzer if the object is too close
  
  if (distanceCm < 10)
   { digitalWrite(led, HIGH);
updateDisplay();
   }
    else digitalWrite(led, LOW);
 
  
    if (distanceCm < 5)
    {digitalWrite(buzzer, HIGH);
updateDisplay();
    }
    else digitalWrite(buzzer, LOW);


   


  
  delay(500);
}


//Issues detecting with angled surfaces
// Alt: store values in list and reference when zero (ie check if something is there)
// Function = Dissappearning Act
