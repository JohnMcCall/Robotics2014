//Motor1 Bank A
#include <Wire.h>
#include <NXShield.h>
//Don't make # symbols
//Bank_A: front-right:1, back-right:2
//Bank_B: front-left: 1, back-left: 2


NXShield nxshield;

void
setup(){
  Serial.begin(115200);
  delay(2000);
  char str[40];
  
  nxshield.init(SH_HardwareI2C);
  
  nxshield.waitForButtonPress(BTN_GO);
  
  nxshield.bank_a.motorReset();
  nxshield.bank_b.motorReset();
}

void
loop(){
  nxshield.ledSetRGB(8,0,0);
  delay(1000);
  nxshield.ledSetRGB(0,8,0);
  delay(1000);
  nxshield.ledSetRGB(0,0,8);
  delay(1000);
  nxshield.ledSetRGB(0,0,0);
  delay(1000);
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
strafeLeft(int speed1){
  nxshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, speed1);
  nxshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Reverse, speed1);
  nxshield.bank_a.motorRunUnlimited(SH_Motor_2, SH_Direction_Reverse, speed1);
}
void
strafeRight(int speed1){
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
