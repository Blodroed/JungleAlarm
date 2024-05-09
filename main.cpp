/**
 * @file main.cpp
 * @author Albert Salvesen-Orø
 */

#include "mbed.h"
#include "libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "include/alarmScreen.h"

#include <ctime>

// alarm buzzer
PwmOut alarmBuzzer(D5);

// Define the LCD object
I2C lcdI2C(D14, D15);
DFRobot_RGBLCD1602 lcd(&lcdI2C);

// Define buttons with interrupts
InterruptIn leftButton(D0, PullUp); 
InterruptIn middleButton(D1, PullUp);
InterruptIn rightButton(D2, PullUp);
InterruptIn specialButton(D4, PullUp);

// Change the screen right or left functions
// This should maybe be added to another class
void changeScreenLeft(int screenNumber, int maxScreenNumber) {
    if (screenNumber == 0) {
        screenNumber = maxScreenNumber;
        return;
    }
    screenNumber--;
}
void changeScreenRight(int screenNumber, int maxScreenNumber) {
    if (screenNumber == maxScreenNumber) {
        screenNumber = 0;
        return;
    }
    screenNumber++;
}


int main()
{
    // Define variables
    int screenNumber = 0;       // you can change this if you want to start on another screen
    int maxScreenNumber = 4;    // max screen should probably be a global variable outside of main

    // initilization
    lcd.init();
    lcd.setRGB(0, 0, 255);  // Set the LCD background color to blue
    set_time(1046703600);   // set RTC to the birth of Albert
    time_t unixtime = time(NULL);

    // Define the alarm screen object
    AlarmScreen alarmScreen(changeScreenLeft, changeScreenRight);

    while (true) {
        switch (screenNumber)
        {
        case 0:
        {
            // Bind buttons to alarm screen
            middleButton.fall(callback(&alarmScreen, &AlarmScreen::middleButtonPressed));
            leftButton.fall(callback(&alarmScreen, &AlarmScreen::leftButtonPressed));
            rightButton.fall(callback(&alarmScreen, &AlarmScreen::rightButtonPressed));
            specialButton.fall(callback(&alarmScreen, &AlarmScreen::specialButtonPressed));

            // screen
            alarmScreen.displayAlarmScreen(lcd);
            break;    
        }
                  
        case 1:
        {
            // Bind buttons to news screen

            // Test screen hardcoded
            lcd.display();
            lcd.printf("Halla Balla");
            lcd.clear();
            break;
        }
        default:
            break;
        }
    }
}
