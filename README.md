# Group-7-Embedded-Systems-2025

Code for Group 7's Sumo Robot's behaviour. The code featured uses sensors to determine the robot's location within the fighting area, that information will instruct the robot on what kind of movement/attack it should use to counter its current predicament. 

Please note that the required code requires the exact same structure, hardware and design used by out team for the project. 
All code written in **C++** is intended to be run on a TTGO controller that will instruct the directed components at the listed ports.
Any plans to reuse or reference this code come with the heavy recommendation to consider the port numbers for input/output. 

# Hardware Used (Electrical Components)
Here are all the electrical components used in our final Sumobot's Design:

- 2x JGA25-370 6V Rated DC Motor (130 rpm, 1kg.cm rated torque, 3kg.cm stall torque) 
- 1x LILYGO T DISPLAY S3 (Rated for approximately 5V voltage input) (TTGO) 
- 2x Colour Sensor GY-31 TCS230 (Rated for 5V) (Front sensor output connected to pin 12, back sensor output connected to pin 13 of the TTGO) 
- 1x HC-SR04 Ultrasonic Sensor. (Rated for 5V) (Trigger pin connected to pin 18, echo connected to pin 17 of TTGO) 
- 1x DRV8833 Motor Driver (which features 2x NMOS H-bridge drivers, with a peak output current of 2A per pin, and capable of handling supply voltages within the 2.7-10.8V range) 
- 1x ADAFRUIT 5V Miniboost buck converter (can step up voltages to 5V and output 1A) 
- 1x 3.7V 1100 mAh LIPO battery  
- 1x 9V lithium ion 9v Battery.  


# Development Pipeline:
## Week 10 [Parts Arrived!]
- Connected and tested parts and motors
- Code for completed for Sensors and motors  
- Consider layout of circuitry and completed required diagrams
- Printed out CAD prototype 
- Begin drafting out behaviour patterns 

## Week 11 [Integration Testing]
- Robot shell completed (mixture of 3D printing and woodwork)
- Finetuning sensor code to contain the following functions
  -   Detect enemy 
  -   Detect floor
  -   Turning left/right
- Internals start to connect in place 
  - Sensors (two sensor for floor, one sensor for enemy detection)
  - Double Motors 

## Week 12 [Reworking Code]
- Robot Tested in Ring once completed
- 1 important sensor failed -> Code rewritten to account for failure
- Successfully achieved 9th place (out of 30) in competition against other students robots 
  
 


