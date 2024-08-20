#ifndef MAIN_H
#define MAIN_H

// Defns
#define DISPLAY_BUTTON_A    9
#define DISPLAY_BUTTON_B    6
#define DISPLAY_BUTTON_C    5

#define M1_PWM_PIN          10 // PWM pin for motor 1
#define M2_PWM_PIN          11 // PWM pin for motor 2
#define M1_DIR_PIN          12 // Direction pin for motor 1
#define STEP_SCALE_M0       4 // Step scale pin for M0
#define STEP_SCALE_M1       1 // Step scale pin for M1

// Motors
#define M1                  1 // Motor 1
#define M2                  2 // Motor 2
// this leaves D12 and D13 left for digital pins

// Function Declarations
void setupMotors();
void setupClock();
void M1setSpeed(int speed);
void M2setSpeed(int speed);
void initTCC0(Tcc *TCC, int period);
void setIOMux(int pin_id, int channel);
void motorTestScript();
void playBadApple(Adafruit_SH1107 display);
void handleButtonPressA();
void handleButtonPressB();
void handleButtonPressC();
void displayMenu();

#endif // MAIN