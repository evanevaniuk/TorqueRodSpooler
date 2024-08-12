#ifndef MENU_H
#define MENU_H

enum MenuState {
  MAIN_MENU,        // The main menu
  MOTOR_MENU,       // The menu for controlling the motors (basically start them)
  MOTOR_PROGRESS_MENU, // The menu for showing the progress of the motors
  SETTINGS_MENU,    // The menu for changing settings/parameters
  ABOUT_MENU,        // press this button 3 times to display Bad Apple. defualt, it should display the Logo and Version number i guess?
  MOTOR_TESTING     // The menu for testing the motors. i want to play an animation here too if possible. 
};


#endif // MENU_H
