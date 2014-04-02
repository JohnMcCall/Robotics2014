#include <Wire.h>
#include <NXShield.h>
#include <Servo.h>

Servo myservo3;

int pos = 0;

void setup(){
  Serial.begin(115200);
  myservo3.attach(3);
}

void loop(){
  pos = 180;
  myservo3.write(pos);
  delay(500);
  pos=90;
  myservo3.write(pos);
  delay(500);
  pos = 0;
  myservo3.write(pos);
  delay(500);
  pos=90;
  myservo3.write(pos);
  delay(5000);
}
