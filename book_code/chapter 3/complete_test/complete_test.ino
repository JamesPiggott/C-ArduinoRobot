#include <Servo.h>

#define Trigger 7
#define Echo 8

Servo servoLeft;
Servo servoRight;

void setup() {
  pinMode(Echo, INPUT);
  pinMode(Trigger, OUTPUT);
  
  servoLeft.attach(9);
  servoRight.attach(10);
}

void loop() {
  measureDistance();
  moveForwards();
  delay(2000);
  moveBackwards();
  delay(2000);
  stopMovement();
  delay(2000);
  turnLeft();
  delay(2000);
  turnRight();
  delay(2000);
}

void measureDistance() {
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);

  digitalWrite(Trigger, LOW);

  int distance = pulseIn(Echo, HIGH);
  distance = distance / 29 / 2; 
}

void moveForwards() {
  servoLeft.write(180);
  servoRight.write(0);
}

void moveBackwards() {
  servoLeft.write(0);
  servoRight.write(180);
}

void stopMovement() {
  servoLeft.write(90);
  servoRight.write(90); 
}

void turnLeft() {
  servoLeft.write(180);
  servoRight.write(180);   
}

void turnRight() {
  servoLeft.write(0);
  servoRight.write(0);   
}
