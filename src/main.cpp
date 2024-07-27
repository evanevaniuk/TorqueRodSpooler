#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "main.h"
#include "badapple.h"

// For testing
#define DEBUG               1 // Debug Mode
#define TEST                0 // Testing Mode

// variables
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void setup() {
  if (DEBUG)  { 
    Serial.begin(115200);
    Serial.println("Debug Mode");
    Serial.println("Starting Torque Rod Spooler");
    Serial.println("128x64 OLED FeatherWing test");
  }
  delay(250); // wait for the OLED to power up
  if (!display.begin(0x3C, true)) {
    if (DEBUG) Serial.println(F("SH1107 allocation failed"));
    while (1); // Don't proceed, loop forever
  }
  // Show the image Buffer of the display hardware. Should be a defualt logo. 
  display.display();  // Consider somehow putting STARLab logo here... 
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clear the display buffer.
  display.setRotation(1); 

  // Set up the buttons. Consider makking them interrupt driven. 
  pinMode(DISPLAY_BUTTON_A, INPUT_PULLUP);
  pinMode(DISPLAY_BUTTON_B, INPUT_PULLUP);
  pinMode(DISPLAY_BUTTON_C, INPUT_PULLUP);

  // Set up the motors
  setupMotors();
} // End setup()


void loop() {
  delay(1000); // wait for a second
  if (TEST) { 
    motorTestScript();
    while (1); // stop the loop
  }
  playBadApple(display);
} // End loop()

/*********************************************/

// Function Definitions

// code to setup motors
void setupMotors() {
  // We need to init 3 pins for the motor driver: 1. Direction pin  2. PWM pin (the Step Pin on the driver) 3. Enable pin
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M1_PWM_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);
  pinMode(M2_PWM_PIN, OUTPUT);
  // And we probably dont need a direction pin on the second motor, since it only needs to go a single direction.  
} // End setupMotors()

// code to spin motor
void spinMotor(int motor, int speed, int direction) {
  // For Phase 1 of testing, We just need both motors to spin. 
  // To spin a motor we select the direction and then the step pin (pwm) is the speed control.
  if (motor == M1) {
    digitalWrite(M1_DIR_PIN, direction);
    analogWrite(M1_PWM_PIN, speed);
  } else if (motor == M2) {
    digitalWrite(M2_DIR_PIN, direction);
    analogWrite(M2_PWM_PIN, speed);
  }
} // End spinMotor()

// code to test motors
void motorTestScript(){ 
  spinMotor(M1, 255, 1);
  spinMotor(M2, 255, 1);
  delay(5000); // wait for 5 seconds  
  spinMotor(M1, 0, 1);
  spinMotor(M2, 0, 1);
  delay(5000); // wait for 5 seconds
  spinMotor(M1, 255, 0);
  spinMotor(M2, 255, 0);
  delay(5000); // wait for 5 seconds
  spinMotor(M1, 0, 0);
  spinMotor(M2, 0, 0);
} // End motorTestScript()


void playBadApple(Adafruit_SH1107 display){
  // This function will play the bad apple animation on the OLED display. 
  for (int i = 0; i < 1095; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, badapple_frames[i], 128, 64, 1);
    display.display();
    delay(100);   
  }
} // End playBadApple()

// End of File