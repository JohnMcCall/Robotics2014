//Motor1 Bank A
#include <Wire.h>
#include <NXShield.h>

NXShield nxshield;

void
setup(){
  Serial.begin(115200);
  delay(2000);
  long rotations = 10;
  char str[40];
  
  nxshield.init(SH_HardwareI2C);
  
  //waiting for go b
  nxshield.waitForButtonPress(BTN_GO);
  
  nxshield.bank_a.motorReset();
  
  delay(1000);
  str[0] = '\0';
  nxshield.bank_a.motorStartBothInSync();
  nxshield.bank_a.motorRunRotations(SH_Motor_1,
                  SH_Direction_Forward,
                  100,
                  rotations,
                  SH_Completion_Wait_For,
                  SH_Next_Action_BrakeHold);
  nxshield.bank_a.motorRunRotations(SH_Motor_1,
                  SH_Direction_Reverse,
                  100,
                  rotations,
                  SH_Completion_Wait_For,
                  SH_Next_Action_BrakeHold);
//  nxshield.bank_b.motorRunRotations(SH_Motor_Both,
//                  SH_Direction_Forward,
//                  100,
//                  rotations,
//                  SH_Completion_Wait_For,
//                  SH_Next_Action_BrakeHold);
    
}
void loop(){
  nxshield.ledSetRGB(8,0,0);
  delay(1000);
  nxshield.ledSetRGB(0,0,0);
  delay(1000);
  
}
#
