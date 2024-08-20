#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "main.h"
#include "badapple.h"
#include "menu.h"
#include "SAMD_PWM.h"

// For testing
#define DEBUG               1 // Debug Mode
#define TEST                0 // Testing Mode

// Global variables
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
volatile MenuState menuState = MAIN_MENU;
volatile bool stateChange = false;

SAMD_PWM* Motor1;
SAMD_PWM* Motor2;

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

  // Set up the step scaling
  pinMode(STEP_SCALE_M0, INPUT);  // input for High-Z
  pinMode(STEP_SCALE_M1, INPUT);

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
  if (DEBUG) {
    Serial.print(F("\nStarting PWM_StepperControl on "));
    Serial.println(BOARD_NAME);
    Serial.println(SAMD_PWM_VERSION);
  }
  Motor1 = new SAMD_PWM(M1_PWM_PIN, 500, 0);
  Motor2 = new SAMD_PWM(M2_PWM_PIN, 500, 0);
  pinMode(M1_DIR_PIN, OUTPUT);
} // End setupMotors()

void M1setSpeed(int speed) {
  if (speed == 0) {
    Motor1->setPWM(M1_PWM_PIN, 500, 0);
  } 
  else { 
    // If the direction is reversed, then we toggle the pin
    digitalWrite(M1_DIR_PIN, (speed < 0));
    Motor1->setPWM(M1_PWM_PIN, abs(speed), 50); // 50% duty cycle
  }
}

void M2setSpeed(int speed) {
  if (speed == 0) {
    Motor2->setPWM(M2_PWM_PIN, 500, 0);
  } 
  else {
    Motor2->setPWM(M2_PWM_PIN, abs(speed), 50); // 50% duty cycle
  }
}

// code to test motors
void motorTestScript(){ 
  // while (!stateChange) {
  //   M1setSpeed(25000);
  //   M2setSpeed(25000);
  // }
  for (int i = 0; i<(200*128); i++) {
    M2setSpeed(1000);
  }
  M1setSpeed(0);
  M2setSpeed(0);
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
    case MOTOR_MENU:
      menuState = MOTOR_TESTING;
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
    case MOTOR_TESTING:
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
      case MOTOR_TESTING:
        display.setTextSize(2);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        display.println("Motor Test:Active");
        display.setTextSize(1);
        display.setCursor(0,50);
        display.println("Return");
        display.display();
        motorTestScript();
        break;
      case ABOUT_MENU:
        // I want to display an actual logo here with some details if possible. then if pressed 3 times, it should display the bad apple animation.
        playBadApple(display);
        break;
    } // End switch
  } // End if stateChange
} // End displayMenu()

void setupClock() {
    // Setup the generic clock 7 (GCLK7) to clock timer TCC0
  GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(1) |          // Divide the 120MHz clock source by divisor 1: 120MHz/1 = 120MHz
                         GCLK_GENCTRL_IDC |              // Set the duty cycle to 50/50 HIGH/LOW
                         GCLK_GENCTRL_GENEN |            // Enable the generator (GCLK7)
                         GCLK_GENCTRL_SRC_DFLL;          // Set the clock source to 120MHz
  while (GCLK->SYNCBUSY.bit.GENCTRL7);                   // Wait for synchronization

  GCLK->PCHCTRL[TCC0_GCLK_ID].reg = GCLK_PCHCTRL_CHEN |       // Enable the tcc0/tcc1 peripheral channel
                                    GCLK_PCHCTRL_GEN_GCLK7;   // connect the 120MHz generic clock 7 to TCO0
  
  setIOMux(M1_PWM_PIN, 2); // Set the PWM pin to channel 2
  
  initTCC0(TCC0, 1000); // Initialize the TCC0 timer with a period of 1000      
}

void initTCC0(Tcc *TCC, int period) {
  MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC0;           // Activate timer TC0

  TCC->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 48MHz/8 = 6MHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock  
  
  TCC->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM; // Set TCC0 to normal PWM mode (NPWM)
  while (TCC->SYNCBUSY.bit.WAVE); // Wait for synchronization

  TCC->PER.reg = period; // Set the period of the PWM
  while (TCC->SYNCBUSY.bit.PER); // Wait for synchronization

  for (int i = 0; i < 6; i++) { // not sure why we are doing this 7 times.
    TCC->CC[i].reg = period/2; // Set the duty cycle
    while (TCC->SYNCBUSY.bit.CC0); // Wait for synchronization
  }

  TCC->CTRLA.bit.ENABLE = 1; // Enable the TCC timer
  while (TCC->SYNCBUSY.bit.ENABLE); // Wait for synchronization
} // End initTCC()

void setIOMux(int pin_id, int channel){ 
  // Enable the peripheral multiplexer on pin_id
  PORT->Group[g_APinDescription[pin_id].ulPort].PINCFG[g_APinDescription[pin_id].ulPin].bit.PMUXEN = 1;
  // Set the pin_id peripheral multiplexer to peripheral (even port number) channel
  if (g_APinDescription[pin_id].ulPin % 2 == 0){
    PORT->Group[g_APinDescription[pin_id].ulPort].PMUX[g_APinDescription[pin_id].ulPin >> 1].reg |= PORT_PMUX_PMUXE(channel);
  } else {  // odd port channel
    PORT->Group[g_APinDescription[pin_id].ulPort].PMUX[g_APinDescription[pin_id].ulPin >> 1].reg |= PORT_PMUX_PMUXO(channel);
  }
} // End setIOMux()

void playBadApple(Adafruit_SH1107 display){
  // This function will play the bad apple animation on the OLED display. 
  for (int i = 0; i < 300; i++) {
    // at any point in the animation, if the state changes, we should stop the animation and back out the menu.
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