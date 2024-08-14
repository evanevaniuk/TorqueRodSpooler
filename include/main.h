#ifndef MAIN_H
#define MAIN_H

// Defns
#define DISPLAY_BUTTON_A    9
#define DISPLAY_BUTTON_B    6
#define DISPLAY_BUTTON_C    5

#define M1_PWM_PIN          10 // PWM pin for motor 1
#define M2_PWM_PIN          11 // PWM pin for motor 2
#define M1_DIR_PIN          12 // Direction pin for motor 1

// Motors
#define M1                  1 // Motor 1
#define M2                  2 // Motor 2
// this leaves D12 and D13 left for digital pins

// Function Declarations
void setupMotors();
void initTCC0(Tcc *TCC, int period);
void setIOMux(int pin_id, int channel);
void spinMotor(int motor, int speed, int direction);
void motorTestScript();
void playBadApple(Adafruit_SH1107 display);
void handleButtonPressA();
void handleButtonPressB();
void handleButtonPressC();
void displayMenu();

#endif // MAIN