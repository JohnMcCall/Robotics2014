//Motor1 Bank A
#include <Wire.h>
#include <NXShield.h>
#include <NXTUS.h>
#include <Servo.h>
#include <SoftwareSerial.h>
//Don't make # symbols
//Bank_A: front-right:1, back-right:2
//Bank_B: front-left: 1, back-left: 2

NXShield nxshield;
NXShield sensorShield;

// 2 line LCD on arduino digital pin #9
SoftwareSerial lcd(2, 9);

//Servo elevatorServo;



//
// declare the i2c devices used on NXShield(s).
//
NXTUS       sonarB; // left
NXTUS       sonarA; // right
NXTUS       sonarBack;

void setup(){
  Serial.begin(115200);
  lcd.begin(9600); 
  lcd.print("Setup Starting...");
  //elevatorServo.attach(3);
  //elevatorStop();
  delay(500);
  
  //
  // some constants?
  //
  int updateDelay = 100; // 100ms sensor / screen update time
  
  nxshield.init(SH_HardwareI2C);
  
  Serial.println("Press GO!");  
  clearDisplay();
  lcd.print("Press GO!");
  nxshield.waitForButtonPress(BTN_GO);
  
  nxshield.bank_a.motorReset();
  nxshield.bank_b.motorReset();
  
  //
  // Initialize the i2c sensors.
  //
  sonarB.init( &nxshield, SH_BBS1 );
  sonarA.init( &nxshield, SH_BAS2 );
  sonarBack.init( &nxshield, SH_BBS2 );
  
  //approachHoop();
  //approachBackWall(20, 5);
  scan(25, 2, 10, 10);  // backWall distance, backThresh, ballThresh, sideThresh

}

void
loop(){

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

// dist - desired distance (~cm) from the back wall of the field.
// backThresh - the threshold (~cm) before the robot will attempt to correct for drift.
// ballThresh - the threshold (~cm) of sudden distance change that will trigger a ball found state.
// sideDist - the distance (~cm) from the sides of the field that the robot will maintain while scanning.

void scan(int dist, int backThresh, int ballThresh, int sideDist) {
  clearDisplay(); 
  lcd.print("BEGIN SCANNING!");
  findCenter(5); // moves the bot to the center
  delay(500);
  //alignLeft(dist, sideDist, backThresh); // move bot to the left of the field
  delay(500);
  approachBackWall(dist, backThresh);
  delay(500);
  
  int curDist = abs(sonarBack.getDist() - dist);
  int rightDist = sonarA.getDist() - sideDist; 
  
  while(curDist < (ballThresh)) {
     curDist = abs(sonarBack.getDist() - dist);
     rightDist = sonarA.getDist() - sideThresh; 
     delay(updateDelay); // sensor / lcd update delay
     
     if(rightDist > 0) {
      lcd.print("PANNING RIGHT!!");
      strafeRight(50);
     } else {
      clearDisplay();
      lcd.print("Right wall reached?");
      delay(200 * sideThresh); // delay allowing robot to run up against wall to square itself...
      approachBackWall(dist, backThresh);
      alignLeft(dist, sideDist, backThresh); // move all the way back to the left when the right wall is reached.
      clearDisplay(); 
     }
     
     if(curDist > backThresh) {
       clearDisplay();
       lcd.print("correcting dist. !");
       delay(100);
       approachBackWall(dist, backThresh);
     }
  }
  clearDisplay();
  lcd.print("found ball!?");
  setLCDCursor(16);
  lcd.print("curDist: ");
  lcd.print(curDist);
  stopMoving();
    
}

// broken right now...
boolean pan(int sideThresh, boolean right) {
  lcd.print("panning! ");
  boolean toReturn = right;
  int dist;
  
  if(toReturn) {
    strafeRight(50); 
    dist = abs(sonarA.getDist() - sideThresh);
  } else {
    strafeLeft(50); 
    dist = abs(sonarB.getDist() - sideThresh);
  }
  
  if(dist < 0) {
   toReturn = !toReturn;
   stopMoving();
   delay(100);
  }
  
  return toReturn;
  
}

// moves the bot to within dist cm, give of take thresh cm, of the back wall
void approachBackWall(int dist, int thresh) {
  clearDisplay();
  lcd.print("approching back wall!");
  
  int curDist = sonarBack.getDist() - dist;
  
  while(abs(curDist) > thresh)  {
    delay(updateDelay);
    curDist = sonarBack.getDist() - dist;
    
    clearDisplay();
    lcd.print("approachBackWall()"); 
    setLCDCursor(16);
    lcd.print("curDist: ");
    lcd.print(curDist);
    
    if (curDist < 0) {
      moveForward(50);
    } else {
      moveBackward(50);      
    }
  }
  
  stopMoving();
  clearDisplay();
}

// centers the robot, and then moves forward till the bot is ~53cm from the hoop wall.
void approachHoop() {
  findCenter(5);
  clearDisplay();
  lcd.write("approachHoop()");
  
  delay(500);
  
  moveForward(50);
  while(sonarBack.getDist() < 53) { 
  }
  
  clearDisplay();
  stopMoving();
}

// a really silly method that just moves the thing left. hopefully. 
void alignLeft(int backDist, int sideDist, int backThresh){
 clearDisplay();
 lcd.write("aligning left...");
 
 int distB = sonarB.getDist() - sideDist;
 int curDist = sonarBack.getDist() - backDist;
 
 strafeLeft(50); 
 
 while(distB > 0) {
  delay(updateDelay); 
  distB = sonarB.getDist() - sideDist; 
  curDist = abs(sonarBack.getDist() - backDist) ;

  clearDisplay();
  lcd.print("Aligning left...");
  setLCDCursor(16);
  lcd.print("left dist: ");
  lcd.print(distB);
  
  if(curDist > backThresh) {
    //stopMoving();
    clearDisplay();
    lcd.write("correcting dist. !");
    delay(100);
    approachBackWall(backDist, 2);
  }
 }
 stopMoving();
 clearDisplay(); 
}

// centers the robot left / right on the field
void findCenter(int threshold) {
  int distA = sonarA.getDist();
  int distB = sonarB.getDist();
  
  clearDisplay();
  lcd.write("findCenter()..");
  
  while(abs(distA - distB) > threshold) {
  delay(updateDelay);
    distA = sonarA.getDist();
    distB = sonarB.getDist();
    
    clearDisplay();
    lcd.print("Finding Center...");
    setLCDCursor(16);
    lcd.print("distA: ");
    lcd.print(distA);
    lcd.print(" distB: ");
    lcd.print(distB);

    if(distA > distB) {
      strafeRight(50); 
    } else {
      strafeLeft(50);
    }
  }
  
  clearDisplay();
  stopMoving();
}


/////////////////////////// MOVEMENT COMMANDS //////////////////////////

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




///////////////////////////// SERVO COMMANDS ////////////////////////////
//void elevatorRaise() {
// elevatorServo.write(0); 
//}
//
//void elevatorLower() {
// elevatorServo.write(180); 
//}
//
//void elevatorStop() {
// elevatorServo.write(90);
//}


///////////////////////////// LCD COMMANDS //////////////////////////////
void clearDisplay() {
  lcd.write(0xFE);
  lcd.write(0x01); 
}

void setLCDCursor(byte cursor_position){
 lcd.write(0xFE); // ready LCD for special command
 lcd.write(0x80); // ready LCD to recieve cursor potition
 lcd.write(cursor_position); // send cursor position 
}
#
