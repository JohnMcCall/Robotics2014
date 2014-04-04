//Motor1 Bank A
#include <Wire.h>
#include <NXShield.h>
#include <NXTUS.h>
#include <Servo.h>
#include <SoftwareSerial.h>
//Don't make # symbols
//Bank_A: front-right:1, back-right:2
//Bank_B: front-left: 1, back-left: 2

SoftwareSerial lcd(2, 8);

Servo elevatorServo;

NXShield nxshield;

//
// declare the i2c devices used on NXShield(s).
//
NXTUS       sonarB; // left
NXTUS       sonarA; // right
NXTUS       sonarBack;

void setup(){
  Serial.begin(115200);
  lcd.begin(9600); 
  lcd.write("Setup Starting...");
  elevatorServo.attach(3);
  delay(500);
  
  nxshield.init(SH_HardwareI2C);
  
  Serial.println("Press GO!");  
  clearDisplay();
  lcd.write("Press GO!");
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
  scan(20, 5, 5, 5);
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

void scan(int dist, int backThresh, int ballThresh, int sideThresh) {
  clearDisplay(); 
  lcd.print("BEGIN SCANNING!");
  findCenter(5); // moves the bot to the center
  delay(500);
  alignLeft(3); // move bot to the left of the field
  delay(500);
  approachBackWall(dist, backThresh);
  delay(500);
  
  int curDist = abs(sonarBack.getDist() - dist);
  int right = true;
  
  clearDisplay(); 
  lcd.print("Entering while loop...");
  while(curDist < (10)) {
     curDist = abs(sonarBack.getDist() - dist);
     int rightDist = sonarA.getDist() - 3; 
     
     
     if(rightDist > 0) {
      strafeRight(50);
     } else {
      clearDisplay();
      lcd.write("Right wall reached?");
      delay(300);
      alignLeft(3); // move all the way back to the left when the right wall is reached.
      clearDisplay(); 
     }
     
     if(curDist > backThresh) {
       approachBackWall(dist, backThresh);
     }
  }
  clearDisplay();
  lcd.print("found ball!?");
  setLCDCursor(16);
  lcd.write("curDist: ");
  lcd.print(curDist);
  stopMoving();
    
}

// brokennnnnnnnnnnnnnnnnnnnnnnnn
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
    curDist = sonarBack.getDist() - dist;
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
  
  delay(500);
  
  moveForward(50);
  while(sonarBack.getDist() < 53) { 
  }
  
  stopMoving();
}

// a really silly method that just moves the thing left. hopefully. 
void alignLeft(int dist){
 clearDisplay();
 lcd.write("aligning left...");
 setLCDCursor(16);
 lcd.write("Dist: ");
 lcd.write(dist);
 
 int distB = sonarB.getDist() - dist;
 
 while(distB > 0) {
  distB = sonarB.getDist() - dist; 
  strafeLeft(50); 
 }
}

// centers the robot left / right on the field
void findCenter(int threshold) {
  clearDisplay();
  lcd.write("Begining: findCenter("); 
  lcd.write(threshold);
  lcd.write(")");
  int distA = sonarA.getDist();
  int distB = sonarB.getDist();
  
  while(abs(distA - distB) > threshold) {
    distA = sonarA.getDist();
    distB = sonarB.getDist();
    
    clearDisplay(); 
    lcd.write("Finding Center...");
    setLCDCursor(16);
    lcd.write("sonarA: ");
    lcd.write(distA);
    lcd.write(" sonarB: ");
    lcd.write(distB);    

    if(distA > distB) {
      strafeRight(50); 
    } else {
      strafeLeft(50);
    }
  }
  
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
void elevatorRaise() {
 elevatorServo.write(0); 
}

void elevatorLower() {
 elevatorServo.write(180); 
}

void elevatorStop() {
 elevatorServo.write(90);
}


///////////////////////////// LCD COMMANDS //////////////////////////////
void clearDisplay() {
  lcd.write(0xFE);
  lcd.write(0x01); 
}

void setLCDCursor(byte cursor_position){
 lcd.write(0xFE); // ready LCD for special command
 lcd.write(0x08); // ready LCD to recieve cursor potition
 lcd.write(cursor_position); // send cursor position 
}
#
