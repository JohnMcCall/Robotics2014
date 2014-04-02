//Motor1 Bank A
#include <Wire.h>
#include <NXShield.h>
#include <NXTUS.h>
//Don't make # symbols
//Bank_A: front-right:1, back-right:2
//Bank_B: front-left: 1, back-left: 2


NXShield nxshield;

//
// declare the i2c devices used on NXShield(s).
//
NXTUS       sonarB;
NXTUS       sonarA;
NXTUS       sonarBack;

void setup(){
  Serial.begin(115200);
  delay(500);
    
  nxshield.init(SH_HardwareI2C);
  
  Serial.println("Press GO!");  
  nxshield.waitForButtonPress(BTN_GO);
  
  nxshield.bank_a.motorReset();
  nxshield.bank_b.motorReset();
  
  //
  // Initialize the i2c sensors.
  //
  sonarB.init( &nxshield, SH_BBS1 );
  sonarA.init( &nxshield, SH_BAS2 );
  sonarBack.init( &nxshield, SH_BBS2 );
  
  approachHoop();
}

void
loop(){
//  strafeLeft(50);
//  Serial.println("Switching to Left");
//  Serial.println(sonarB.getDist());
//  while(sonarB.getDist()>5){ 
//    Serial.println("StrafingLeft!");
//    Serial.println(sonarB.getDist());
//  }
//// stopMoving();
// strafeRight(50);
// Serial.println("Switching to Right");
// Serial.println(sonarA.getDist());
//  while(sonarA.getDist()>5){
//    Serial.println("StrafingRight!");
//    Serial.println(sonarA.getDist());
//  }
//  coloryLights();
}

void
coloryLights(){
  nxshield.ledSetRGB(8,0,0);
  delay(1000);
  nxshield.ledSetRGB(0,8,0);
  delay(1000);
  nxshield.ledSetRGB(0,0,8);
  delay(1000);
  nxshield.ledSetRGB(0,0,0);
  delay(1000);
}

void approachHoop() {
  findCenter(5);
  
  moveForward(50);
  while(sonarBack.getDist() < 53) { 
  }
  
  stopMoving();
}

void findCenter(int threshold) {
  int distA = sonarA.getDist();
  int distB = sonarB.getDist();
  
  while(abs(distA - distB) > threshold) {
    distA = sonarA.getDist();
    distB = sonarB.getDist();
    
    if(distA > distB) {
      strafeRight(50); 
    } else {
      strafeLeft(50); 
    }
  }
  
  stopMoving();
}

void
moveForward(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
}
void
moveBackward(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
}
void
stopMoving(){
  nxshield.bank_a.motorStop(SH_Motor_Both, SH_Next_Action_Float);
  nxshield.bank_b.motorStop(SH_Motor_Both, SH_Next_Action_Float);
}
void
strafeRight(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
}
void
strafeLeft(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
  
}
void
turnClockwise(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
}
void
turnCounterClockwise(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
}

void
diagonal(){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, 40);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, 90);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 90);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, 40);
  
}
#
