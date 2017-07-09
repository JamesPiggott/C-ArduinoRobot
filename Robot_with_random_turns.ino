/**********************************
* Author: James Piggott           *
* Modified: 24/2/17               *
**********************************/

#include <Servo.h>

// create servo objects
Servo leftMotor;
Servo rightMotor;

int leftMotorForwardSpeed   = 55;
int rightMotorForwardSpeed  = 150;

int ultrasonic1Distance;
int ultrasonic1Duration;

int ultrasonic1TimeObjectDetected  = 0;
int ultrasonic2TimeWallNotDetected = 0;

int wallInFront   = 0;
int wallOnLeft    = 0;
int turnCompleted = 0;

unsigned long timeSerialDelay     = 0;
unsigned long timeUltrasonicDelay = 0;

unsigned long timeTurnInitiated = 0;
unsigned long time90DegTurn     = 690;

unsigned long timeDriveForwardAfterTurn  = 2000;
unsigned long timeDriveForwardBeforeTurn = 1000;
unsigned long timeStartedDrivingForward  = 0;

// specify the trig & echo pins used for the ultrasonic sensors (these really should be #defines, but Arduino won't allow it for some reason)
int ULTRASONIC_1_TRIG_PIN = 5;
int ULTRASONIC_1_ECHO_PIN = 6;

// define the states
#define DRIVE_FORWARD     0
#define TURN_LEFT         1
#define TURN_RIGHT        2

int state = DRIVE_FORWARD; // 0 = drive forward (DEFAULT), 1 = turn left, 2 = turn right

void setup() {
    Serial.begin(9600);
  
    // pin configurations
    pinMode(ULTRASONIC_1_TRIG_PIN, OUTPUT); // ultrasonic 1
    pinMode(ULTRASONIC_1_ECHO_PIN, INPUT);  // ultrasonic 1
    
    pinMode(2, OUTPUT); // green LED
    pinMode(3, OUTPUT); // red LED
  
    rightMotor.attach(12); // attaches the servo on pin 12 to the servo object 
    leftMotor.attach(13);  // attaches the servo on pin 13 to the servo object 
}

void loop() {
      debugOutput();
      readUltrasonicSensors(); // read and store the measured distances
      drivingStates();
}

void readUltrasonicSensors()
{
    if((millis() - timeUltrasonicDelay) > 50) // only get the distance every 50ms (the ultrasonics make noise if there's no delay)
    {
      // ultrasonic 1
      digitalWrite(ULTRASONIC_1_TRIG_PIN, HIGH);
      delayMicroseconds(10);                      // must keep the trig pin high for at least 10us
      digitalWrite(ULTRASONIC_1_TRIG_PIN, LOW);
    
      ultrasonic1Duration = pulseIn(ULTRASONIC_1_ECHO_PIN, HIGH); // starts timing when the pin goes high and stops timing when the pin goes low
      ultrasonic1Distance = ultrasonic1Duration/74/2;             // calculate the distance in inches: the speed of sound is 340 m/s or 29 microseconds per centimeter.
    
      timeUltrasonicDelay = millis();
    }
}

void drivingStates() {
//    state = DRIVE_FORWARD;
//    if (ultrasonic1Distance > 4) {
//      state = DRIVE_FORWARD;
//    } else {
//      state = TURN_LEFT;
//    }

    if(wallInFront == 1) {
      state = TURN_LEFT;
      wallInFront = 0;
      wallOnLeft = 1;
    } else if((wallInFront == 1) && (wallOnLeft == 1)) {
      state = TURN_RIGHT;
      wallInFront = 0;
      wallOnLeft = 0;
    }
        
    // green LED on (red LED off) while driving forward
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
  
    if(state == DRIVE_FORWARD) { 
      if(ultrasonic1Distance > 10 || ultrasonic1Distance <= 0) {
        ultrasonic1TimeObjectDetected = 0;    // no object detected
        wallInFront = 0;                      // no wall in front
              
        rightMotor.write(rightMotorForwardSpeed);
        leftMotor.write(leftMotorForwardSpeed);
      } else {
        if(ultrasonic1TimeObjectDetected == 0) {
          ultrasonic1TimeObjectDetected = millis(); // the object was detected now
        }
        if((millis() - ultrasonic1TimeObjectDetected) > 150) {
          wallInFront = 1;
        }
    }

    if(state == TURN_LEFT) { // no obstacles detected
       if(timeTurnInitiated == 0) {
          timeTurnInitiated = millis();
          rightMotor.write(180);
          leftMotor.write(180);
        }
        if((millis() - timeTurnInitiated) > time90DegTurn) {
          state = DRIVE_FORWARD;
          timeTurnInitiated = 0;
        }
        // red LED on (green LED off) while turning
        digitalWrite(2, LOW);  
        digitalWrite(3, HIGH);
    }
  
    if(state == TURN_RIGHT) {
       if(timeTurnInitiated == 0) {
          timeTurnInitiated = millis();
          rightMotor.write(180);
          leftMotor.write(180);
        }
        if((millis() - timeTurnInitiated) > time90DegTurn) {
          state = DRIVE_FORWARD;
          timeTurnInitiated = 0;
        }
        // red LED on (green LED off) while turning
        digitalWrite(2, LOW);  
        digitalWrite(3, HIGH);
    }
  }
}
void debugOutput() {
    if((millis() - timeSerialDelay) > 250) {
        Serial.print("ultrasonic 1: ");
        Serial.print(ultrasonic1Distance);
        Serial.print("in");
        Serial.println();    

        if(state == DRIVE_FORWARD)
          Serial.println("State: DRIVE_FORWARD");
        else if(state == TURN_LEFT)
          Serial.println("State: TURN_LEFT");
        else if(state == TURN_RIGHT)
          Serial.println("State: TURN_LEFT");
        timeSerialDelay = millis();
    }
}




