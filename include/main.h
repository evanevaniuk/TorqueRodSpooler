#ifndef MAIN_H
#define MAIN_H

// Defns
#define DISPLAY_BUTTON_A    9
#define DISPLAY_BUTTON_B    6
#define DISPLAY_BUTTON_C    5

#define M1_DIR_PIN          4 // Direction pin for motor 1
#define M1_PWM_PIN          5 // PWM pin for motor 1
#define M2_DIR_PIN          9 // Direction pin for motor 2
#define M2_PWM_PIN          10 // PWM pin for motor 2

// For testing
#define DEBUG               1 // Debug Mode
#define TEST                1 // Testing Mode

// Motors
#define M1                  1 // Motor 1
#define M2                  2 // Motor 2
// this leaves D12 and D13 left for digital pins

// Function Declarations
void setupMotors();
void spinMotor(int motor, int speed, int direction);
void motorTestScript();


#endif // MAIN