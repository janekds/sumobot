#include <Servo.h>
             
Servo LRServo;         // set a variable to map the servo
int LRPos = 0;         // set a variable to store the servo position
const int numReadings = 1;   // set a variable for the number of readings to take
int index = 0;                // the index of the current reading
int total = 0;                // the total of all readings
int average = 0;              // the average
int echoPin = 7;              // the SRF05's echo pin
int trigPin = 8;              // the SRF05's init pin
unsigned long pulseTime = 0;  // variable for reading the pulse
unsigned long distance = 0;   // variable for storing distance
int maxDist = 20; //arena length
int leftDrive1 = 2;
int leftDrive2 = 3;
int rightDrive1 = 4;
int rightDrive2 = 5;
int servo = 0;
int found = false;

 
/* setup the pins, servo and serial port */
void setup() {
  LRServo.attach(9);
  // make the init pin an output:
  pinMode(trigPin, OUTPUT);
  // make the echo pin an input:
  pinMode(echoPin, INPUT);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(leftDrive1, OUTPUT);
  pinMode(leftDrive2, OUTPUT);
  pinMode(rightDrive1, OUTPUT);
  pinMode(rightDrive2, OUTPUT);
} 
 
/* begin rotating the servo and getting sensor values */
void loop() {
  test();
}

int radar(){
      int found = false;
      for(LRPos = 0; LRPos < 210; LRPos= LRPos+10){  // going left to right.
      LRServo.write(LRPos);
      delay(150);
      distance = getDistance();
      Serial.println(distance);
      if (distance < maxDist)  {
        Serial.println("target found");
        LRServo.write(0);
        
        found = true;
        return LRPos, found;
      }
    }
    return LRPos, found;
}

  
int getDistance() {
   digitalWrite(trigPin, LOW);
   delayMicroseconds(50);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(50);
   digitalWrite(trigPin, LOW);
   pulseTime = pulseIn(echoPin, HIGH);
   distance = pulseTime/58;
   return distance;
}

void MoveForward() {
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, HIGH);
  digitalWrite(rightDrive1 , HIGH);
  digitalWrite(rightDrive2, LOW);
}

void MoveBackward(){
   digitalWrite(leftDrive1, HIGH);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , LOW);
  digitalWrite(rightDrive2, HIGH);
}
void StopMoving(){
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , LOW);
  digitalWrite(rightDrive2, LOW);
}

void rotate(int angle){
  // later add time using imu to determine angle of movement
  digitalWrite(leftDrive1, HIGH);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , HIGH);
  digitalWrite(rightDrive2, LOW);
  delay(500);
  digitalWrite(leftDrive1, LOW);
  digitalWrite(rightDrive1, LOW);
}

void test(){
     servo, found = radar();
   while (found == false){
      Serial.println("false");
      rotate(90);
      Serial.println("rotating");
      delay(1000);
      StopMoving();
      servo, found = radar();
      if (found == true){
        break;
      }
  }
  //rotate using imu
  distance = getDistance();
  LRServo.write(90);
  MoveForward();
  while (distance <= maxDist){
    Serial.println("tracking");
    MoveForward();
    distance = getDistance();
    delay(100);
  StopMoving();
  //MoveBackward();
 // servo, found = radar();
  //Serial.println(servo);
  //rotate(90);
}
}
