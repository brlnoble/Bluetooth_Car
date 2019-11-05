//Steering and Speed Control
#include <Servo.h>
Servo rservo;
Servo lservo;

#define b1 A0 //input pins from Pi
#define b2 A1
#define b3 A2
#define b4 A3

#define mA  8 //basic motor pins
#define ma  9
#define enA  5
#define mB  10
#define mb  11
#define enB  6

int mSpeed = 50; //default slow speed, may need to be increased after testing

int bits[] = {0,0,0,0}; //from RPi

int pos = 90; //default for servos, straight ahead for steering

void setup() {
  pinMode(mA, OUTPUT);
  pinMode(ma, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(mB, OUTPUT);
  pinMode(mb, OUTPUT);
  pinMode(enB, OUTPUT);
  
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  
  digitalWrite(mA, LOW); //default motors off
  digitalWrite(ma, LOW);
  digitalWrite(mB, LOW);
  digitalWrite(mb, LOW);
  
  digitalWrite(b1, HIGH); //remove any noise by creating a pull up network
  digitalWrite(b2, HIGH);
  digitalWrite(b3, HIGH);
  digitalWrite(b4, HIGH);
  
  analogWrite(enA, 0); //default motors off until started
  analogWrite(enB, 0);
  rservo.attach(13); //servo pins for controlling steering
  lservo.attach(12);
  
  rservo.write(pos); //default servos to straight
  lservo.write(pos);
}

void loop() {
  if(digitalRead(b1) == HIGH) { //collects input from RPi first, assigning it to a small array for easy access
    bits[0] = 1;
  }
  else if(digitalRead(b1) == LOW) {
    bits[0] = 0;
  }
  if(digitalRead(b2) == HIGH) {
    bits[1] = 1;
  }
  else if(digitalRead(b2) == LOW) {
    bits[1] = 0;
  }
  if(digitalRead(b3) == HIGH) {
    bits[2] = 1;
  }
  else if(digitalRead(b3) == LOW) {
    bits[2] = 0;
  }
  if(digitalRead(b4) == HIGH) {
    bits[3] = 1;
  }
  else if(digitalRead(b4) == LOW) {
    bits[3] = 0;
  }


  if(bits[3] == 1 && bits[2] == 1) { //forwards
   digitalWrite(mA, HIGH);
   digitalWrite(mB, HIGH);
   analogWrite(enA, mSpeed);
   analogWrite(enB, mSpeed); 
  }
  else if(bits[3] == 0 && bits[2] == 0) { //stop
   digitalWrite(mA, LOW);
   digitalWrite(mB, LOW);
   analogWrite(enA, 0);
   analogWrite(enB, 0); 
  }

  if(bits[3] == 1 && bits[2] == 0) { //increase speed
    mSpeed += 20;
    if(mSpeed > 250) { //max speed
      mSpeed = 250;
    }
  }
  else if(bits[3] == 0 && bits[2] == 1) { //decrease speed
    mSpeed -= 20;
    if(mSpeed < 50) { //min speed
      mSpeed = 50;
    }
  }
  
  if(bits[1] == 1 && bits[0] == 0) { //steer to the right
    pos += 5;
    if(pos >= 135) {
      pos = 135;
    }
  }
  else if(bits[1] == 0 && bits[0] == 1) { //steer to the left
    pos -= 5;
    if(pos <= 45) {
      pos = 45;
    }
  }
  else if(bits[1] == 1 && bits[0] == 1) { //KILL SWITCH
    digitalWrite(mA, LOW);
    digitalWrite(ma, LOW);
    digitalWrite(mB, LOW);
    digitalWrite(mb, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    rservo.write(90);
    lservo.write(90);
  }
  else { //when steering is released, return gradually to default position
    if(pos < 90) { //return from left turn
      pos += 5;
    }
    else if(pos > 90) { //return from right turn
      pos -= 5;
    }
    else { //at default
      pos = 90;
    }
  }
  rservo.write(pos);
  lservo.write(pos);
  analogWrite(enA, mSpeed);
  analogWrite(enB, mSpeed);
  delay(50);
}

