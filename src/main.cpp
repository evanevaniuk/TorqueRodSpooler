#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "main.h"
#include "badapple.h"
#include "menu.h"

// For testing
#define DEBUG               1 // Debug Mode
#define TEST                0 // Testing Mode

// Global variables
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
volatile MenuState menuState = MAIN_MENU;
volatile bool stateChange = false;


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
  // Show the image Buffer of the display hardware
  display.display();  // Displays my new splash screen
  delay(2000); // Pause for 2 seconds. really soak it in...
  display.clearDisplay(); // Clear the display buffer.
  display.setRotation(1); 

  // Initialize button pins with internal pull-up resistors
  pinMode(DISPLAY_BUTTON_A, INPUT_PULLUP);
  pinMode(DISPLAY_BUTTON_B, INPUT_PULLUP);
  pinMode(DISPLAY_BUTTON_C, INPUT_PULLUP);

  // Setup button interrupt
  attachInterrupt(digitalPinToInterrupt(DISPLAY_BUTTON_A), handleButtonPressA, FALLING);
  attachInterrupt(digitalPinToInterrupt(DISPLAY_BUTTON_B), handleButtonPressB, FALLING);
  attachInterrupt(digitalPinToInterrupt(DISPLAY_BUTTON_C), handleButtonPressC, FALLING);

  // Set up the motors
  setupMotors();

  // Now display the Main menu
  stateChange = true;
  displayMenu();  // state is set to main menu by default. 
} // End setup()


void loop() {
  delay(10);
  if (TEST) { 
    motorTestScript();
    while (1); // stop the loop
  }
  // Main loop
  displayMenu();
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

void handleButtonPressA(){ 
  // This should be the callback function for A button presses. 
  if (DEBUG) Serial.println("Button A Pressed");
  switch(menuState) {
    case MAIN_MENU: 
      menuState = MOTOR_MENU;
      break;
    case MOTOR_MENU:
      menuState = MOTOR_PROGRESS_MENU;
      break;
    default:
      menuState = MAIN_MENU;
      break;
  }
  stateChange = true;
} // End handleButtonPresses()

void handleButtonPressB(){ 
  // This should be the callback function for B button presses. 
  if (DEBUG) Serial.println("Button B Pressed");
  switch(menuState) {
    case MAIN_MENU: 
      menuState = SETTINGS_MENU;
      break;
    default:
      menuState = MAIN_MENU;
      break;
  }
  stateChange = true;
} // End handleButtonPresses()

void handleButtonPressC(){ 
  // This should be the callback function for C button presses. 
  // C button should act like our back button for everything except the main menu.
  if (DEBUG) Serial.println("Button C Pressed");
  switch(menuState) {
    case MAIN_MENU: 
      menuState = ABOUT_MENU;
      break;
    case MOTOR_MENU:
      menuState = MAIN_MENU;
      break;
    case MOTOR_PROGRESS_MENU:
      menuState = MOTOR_MENU;
      break;
    case SETTINGS_MENU:
      menuState = MAIN_MENU;
      break;
    case ABOUT_MENU:
      menuState = MAIN_MENU;
      break;
  }
  stateChange = true;
} // End handleButtonPresses()

void displayMenu(){ 
  display.clearDisplay();
  if (stateChange) { 
    stateChange = false;  // reset the stateChange flag
    switch(menuState) {
      case MAIN_MENU: 
        // Move title to the right side and split into two lines
        display.setTextSize(2); // Set text size to 2
        display.setTextColor(SH110X_WHITE);
        display.setCursor(80, 0); // Adjust the x position to move to the right
        display.println("Main");
        display.setCursor(80, 20); // Adjust the y position for the second line
        display.println("Menu");

        display.setTextSize(1); // Set text size to 1 for the button options
        display.setCursor(0, 10); // Position for the first button option
        display.println("Motors");

        display.setCursor(0, 30); // Position for the second button option
        display.println("Settings");

        display.setCursor(0, 50); // Position for the third button option
        display.println("About");

        display.display(); // Display the updated content
        break;
      case MOTOR_MENU:
        // Move title to the right side and split into two lines
        display.setTextSize(2); // Set text size to 2
        display.setTextColor(SH110X_WHITE);
        display.setCursor(68, 0); // Adjust the x position to move to the right
        display.println("Motor");
        display.setCursor(80, 20); // Adjust the y position for the second line
        display.println("Menu");

        display.setTextSize(1); // Set text size to 1 for the button options
        display.setCursor(0, 10); // Position for the first button option
        display.println("Start");

        display.setCursor(0, 30); // Position for the second button option
        display.println("Set Params");

        display.setCursor(0, 50); // Position for the third button option
        display.println("Return");

        display.display(); // Display the updated content
        break;
      case MOTOR_PROGRESS_MENU:
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        display.println("Motor Progress Menu");
        display.display();
        break;
      case SETTINGS_MENU:
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        display.println("Settings Menu");
        display.display();
        break;
      case ABOUT_MENU:
        playBadApple(display);
        break;
    } // End switch
  } // End if stateChange
} // End displayMenu()

void playBadApple(Adafruit_SH1107 display){
  // This function will play the bad apple animation on the OLED display. 
  for (int i = 0; i < 300; i++) {
    if (stateChange) {
        display.clearDisplay();
        display.display();
        return;
    }
    display.clearDisplay();
    display.drawBitmap(0, 0, badapple_frames[i], 128, 64, 1);
    display.display();
    delay(100);   
  }
  display.clearDisplay();
  display.display();
} // End playBadApple()

// End of File