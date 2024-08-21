#ifndef MAIN_H
#define MAIN_H

// Macros
#define DEBUG               0 // Debug Mode
#define TEST                0 // Testing Mode
#define M1                  1 // Motor 1
#define M2                  2 // Motor 2

#define STEP_SCALE_M0       4 // Step scale pin for M0
#define STEP_SCALE_M1       1 // Step scale pin for M1
#define ENABLE_PIN          0 // Enable pin for both motor drivers

#define DISPLAY_BUTTON_A    9
#define DISPLAY_BUTTON_B    6
#define DISPLAY_BUTTON_C    5

#define M1_PWM_PIN          10 // PWM pin for motor 1
#define M2_PWM_PIN          11 // PWM pin for motor 2
#define M1_DIR_PIN          12 // Direction pin for motor 1

// Remaining Digital pins: D0, D13. maybe more i dont understand the schematic very well. 

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