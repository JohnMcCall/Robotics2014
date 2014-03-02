#include <Wire.h>
#include <NXShield.h>
#include <NXTUS.h>

// setup for this example:
// attach external power to NXShield.
// attach LEGO Ultrasonic sensors to BAS2 and BBS2
// Open the Serial terminal to view.


//
// declare the NXShield(s) attached to your Arduino.
//
NXShield    nxshield;

//
// declare the i2c devices used on NXShield(s).
//
NXTUS       sonar1;
NXTUS       sonar2;

void setup() {

  Serial.begin(115200);  // start serial for output
  delay(500); // wait, allowing time to activate the serial monitor

  Serial.println (__FILE__);
  Serial.println ("Initializing the devices ...");
  //
  // Initialize the protocol for NXShield
  // It is best to use Hardware I2C (unless you want to use Ultrasonic).
  //
  nxshield.init( SH_SoftwareI2C );

  //
  // Wait until user presses GO button to continue the program
  //
  Serial.println ("Press GO button to continue");
  nxshield.waitForButtonPress(BTN_GO);

  //
  // Initialize the i2c sensors.
  //
  sonar1.init( &nxshield, SH_BBS1 );
  sonar2.init( &nxshield, SH_BAS1 );
}
  
void loop() {
 
 char str1[256];
 //char str2[256];
 
 sprintf (str1, "sonar1: %d", sonar1.getDist());
 //sprintf (str2, "sonar2: %d", sonar2.getDist());
 
 Serial.println(str1);
 delay(1000);
 //Serial.println(str2);
 
}
  
