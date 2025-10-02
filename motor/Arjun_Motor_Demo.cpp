/**
Features the makings of the robot auto-behaviour we want for our final build
- Requires testing and editing still
*/

#include <Arduino.h>
 
// --- Motor pins (adjust for your wiring) ---

#define MOTOR_PWM  25   // PWM pin connected to motor driver ENA/ENB

#define MOTOR_DIR  26   // Direction pin connected to motor driver IN1
 
// --- PWM setup ---

#define PWM_CHANNEL   0

#define PWM_FREQ      5000    // 5 kHz PWM frequency

#define PWM_RESOLUTION 8      // 8-bit (0-255 duty cycle)
 
void setup() {

  Serial.begin(115200);
 
  // Setup motor direction pin

  pinMode(MOTOR_DIR, OUTPUT);

  digitalWrite(MOTOR_DIR, HIGH);  // set initial direction
 
  // Setup PWM

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(MOTOR_PWM, PWM_CHANNEL);
 
  Serial.println("Motor control ready.");

}
 
void loop() {

  // Ramp motor speed up

  for (int speed = 0; speed <= 255; speed += 25) {

    ledcWrite(PWM_CHANNEL, speed);  // set speed

    Serial.printf("Speed = %d\n", speed);

    delay(1000);

  }
 
  // Ramp motor speed down

  for (int speed = 255; speed >= 0; speed -= 25) {

    ledcWrite(PWM_CHANNEL, speed);

    Serial.printf("Speed = %d\n", speed);

    delay(1000);

  }
 
  // Reverse direction

  digitalWrite(MOTOR_DIR, !digitalRead(MOTOR_DIR));

  delay(2000);

}

 
Distance sensor code:
"#include <Arduino.h>
 
// --- Motor pins ---

#define MOTOR_PWM  25   // Motor PWM pin

#define MOTOR_DIR  26   // Motor direction pin
 
// --- PWM setup ---

#define PWM_CHANNEL   0

#define PWM_FREQ      5000

#define PWM_RESOLUTION 8   // 0-255 duty
 
// --- Ultrasonic sensor pins ---

#define TRIG_PIN  32

#define ECHO_PIN  33
 
// --- Distance threshold (cm) ---

#define DIST_THRESHOLD 20  // if object is closer than 20cm → move
 
void setup() {

  Serial.begin(115200);
 
  // Motor setup

  pinMode(MOTOR_DIR, OUTPUT);

  digitalWrite(MOTOR_DIR, HIGH); // forward direction

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(MOTOR_PWM, PWM_CHANNEL);
 
  // Ultrasonic setup

  pinMode(TRIG_PIN, OUTPUT);

  pinMode(ECHO_PIN, INPUT);
 
  Serial.println("System ready.");

}
 
long readDistanceCM() {

  // Send ultrasonic pulse

  digitalWrite(TRIG_PIN, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);
 
  // Read echo

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout at ~30ms (~5m range)
 
  // Convert to cm

  long distance = duration * 0.034 / 2;

  return distance;

}
 
void loop() {

  long distance = readDistanceCM();

  Serial.printf("Distance: %ld cm\n", distance);
 
  if (distance > 0 && distance < DIST_THRESHOLD) {

    // Object detected → move forward

    ledcWrite(PWM_CHANNEL, 200);  // set speed (0-255)

    Serial.println("Response: 1 (Forward)");

  } else {

    // No object nearby → stop

    ledcWrite(PWM_CHANNEL, 0);

    Serial.println("Response: 0 (Stop)");

  }
 
  delay(200);

}

"
 
