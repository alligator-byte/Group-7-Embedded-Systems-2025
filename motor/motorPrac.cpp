//Missing Include files required
//Should just spin a wheel straight up

// Simple motor spin example

const int motorPWM = 5;   // PWM pin connected to H-bridge motor speed input
const int motorDir = 4;   // Direction pin connected to H-bridge

void setup() {
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDir, OUTPUT);

  // Set motor direction forward
  digitalWrite(motorDir, HIGH);

  // Start motor at half speed (range 0-255)
  analogWrite(motorPWM, 128);
}

void loop() {
  // Nothing needed here, motor runs continuously
}
