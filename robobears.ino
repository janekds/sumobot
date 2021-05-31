#include <Servo.h>
#include <TimerOne.h>
             
Servo LRServo;         // set a variable to map the servo
int LRPos = 0;         // set a variable to store the servo position
const int numReadings = 1;   // set a variable for the number of readings to take
int index = 0;                // the index of the current reading
int total = 0;                // the total of all readings
int average = 0;              // the average
int echoPin = 12;              // the SRF05's echo pin
int trigPin = 11;              // the SRF05's init pin
unsigned long pulseTime = 0;  // variable for reading the pulse
unsigned long distance = 0;   // variable for storing distance
int maxDist = 20; //arena length
int leftDrive1 = 6;
int leftDrive2 = 7;
int rightDrive1 = 4;
int rightDrive2 = 5;
int servo = 0;
int found = false;
const byte MOTOR_R = 3;  // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_L = 2;
const float stepcount = 20.00;  // 20 Slots in disk, change if different
 
// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters, change if different
 
// Integers for pulse counters
volatile int counter_R = 0;
volatile int counter_L = 0;
void ISR_countR()  
{
  counter_R++;// increment Motor A counter value

  
} 
 
// Motor B pulse count ISR
void ISR_countL()  
{
  counter_L++;  // increment Motor B counter value
}
 
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
  counter_R = 0;
  counter_L = 0;
  attachInterrupt(digitalPinToInterrupt (MOTOR_R), ISR_countR, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_L), ISR_countL, RISING);  // Increase counter 2 when speed sensor pin goes High
  LRServo.write(90);
}
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
        found = true;
        servo = LRPos;
        break;
      }
    }
    return found;
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

int AngleToStep(int ang){
  /* 90 deg = 15 steps roughly */
  int steps;
  steps = (15*ang)/90;
  Serial.println("rotate by ");
  Serial.print(steps);
  return steps;
}

void MoveForward() {
  digitalWrite(leftDrive1, HIGH);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , HIGH);
  digitalWrite(rightDrive2, LOW);
}

void MoveBackward(){
   digitalWrite(leftDrive1, HIGH);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , HIGH);
  digitalWrite(rightDrive2, LOW);
}
void StopMoving(){
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, LOW);
  digitalWrite(rightDrive1 , LOW);
  digitalWrite(rightDrive2, LOW);
}
  

void rotateLeft( int steps ){
  counter_R=0;
  counter_L =0;
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, HIGH);
  
  digitalWrite(rightDrive1 , HIGH);
  digitalWrite(rightDrive2, LOW);
  while (steps > counter_R || steps > counter_L) {
   
    if (steps < counter_R) {
    digitalWrite(rightDrive1, LOW);
    digitalWrite(rightDrive2, LOW);
    } 
    if (steps < counter_L) {
    digitalWrite(leftDrive1, LOW);
    digitalWrite(rightDrive2, LOW);
    } 
   }
  //Serial.println(counter_R);
  //Serial.println(counter_L);
  digitalWrite(rightDrive1, LOW);
  digitalWrite(rightDrive2, LOW);
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, LOW); 
  counter_R = 0;  //  reset counter R to zero
  counter_L = 0;  //  reset counter L to zero 
}

void rotateRight( int steps ){
  counter_R=0;
  counter_L =0;
  digitalWrite(leftDrive1, HIGH);
  digitalWrite(leftDrive2, LOW);
  
  digitalWrite(rightDrive1 , LOW);
  digitalWrite(rightDrive2, HIGH);
  while (steps > counter_R || steps > counter_L) {
   
    if (steps < counter_R) {
    digitalWrite(rightDrive1, LOW);
    digitalWrite(rightDrive2, LOW);
    } 
    if (steps < counter_L) {
    digitalWrite(leftDrive1, LOW);
    digitalWrite(rightDrive2, LOW);
    } 
   }
  //Serial.println(counter_R);
  //Serial.println(counter_L);
  digitalWrite(rightDrive1, LOW);
  digitalWrite(rightDrive2, LOW);
  digitalWrite(leftDrive1, LOW);
  digitalWrite(leftDrive2, LOW); 
  counter_R = 0;  //  reset counter R to zero
  counter_L = 0;  //  reset counter L to zero 
}

void test(){
   found = radar();
   while (found == false){
      Serial.println("false");
      rotateLeft(20);
      Serial.println("rotating");
      delay(2000);
      StopMoving();
      found = radar();
      if (found == true){
        Serial.println("angle");
        Serial.println(servo);
        break;
      }
  }
  delay(2000);
  if (servo>90){
    rotateLeft( AngleToStep(servo - 90));
  }else {
    rotateRight( AngleToStep(90 - servo));
  }
  delay(2000);
  LRServo.write(90);
  MoveForward();
  delay(2000);
  while (distance <= maxDist){
    Serial.println("tracking");
    MoveForward();
    distance = getDistance();
    delay(100);
  StopMoving();
  
}}
