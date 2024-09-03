#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Macros
#define DEBUG               1 // Debug Mode
#define TEST                0 // Testing Mode
#define M1                  1 // Motor 1
#define M2                  2 // Motor 2

#define STEP_SCALE_M0       0 // Step scale pin for M0
#define STEP_SCALE_M1       1 // Step scale pin for M1
#define ENABLE_PIN          10 // Enable pin for both motor drivers

#define DISPLAY_BUTTON_A    9
#define DISPLAY_BUTTON_B    6
#define DISPLAY_BUTTON_C    5

#define M1_PWM_PIN          4 // PWM pin for motor 1
#define M2_PWM_PIN          11 // PWM pin for motor 2
#define M1_DIR_PIN          12 // Direction pin for motor 1

// Remaining Digital pins: D0, D13. maybe more i dont understand the schematic very well. 

// Enumerations
enum MENUSTATE {
  MAIN_MENU,        // The main menu
  MOTOR_MENU,       // The menu for controlling the motors (basically start them)
  MOTOR_PROGRESS_MENU, // The menu for showing the progress of the motors
  SETTINGS_MENU,    // The menu for changing settings/parameters
  ABOUT_MENU1,        // press this button 3 times to display Bad Apple. defualt, it should display the Logo and Version number i guess?
  MOTOR_TESTING,     // The menu for testing the motors. i want to play an animation here too if possible. 
  ABOUT_MENU2       // The menu for displaying the Bad Apple animation
};

// Global Variables
extern volatile bool stateChange; // Flag to indicate a change in the menu state
extern volatile MENUSTATE mState; // The current menu state
extern Adafruit_SH1107 display;

// Function Declarations
void setupMotors();
void M1setSpeed(int speed);
void M2setSpeed(int speed);
void motorTestScript();
void playBadApple(Adafruit_SH1107 display);
void handleButtonPressA();
void handleButtonPressB();
void handleButtonPressC();
void displayMenu();

#endif // MAIN