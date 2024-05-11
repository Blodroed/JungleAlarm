/**
 * @file main.cpp
 * @author Albert Salvesen-Orø
 */

#include "mbed.h"
#include "libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "include/alarmScreen.h"

#include <algorithm>
#include <ctime>
#include <exception>

#include "buttonHandler.h"

// alarm buzzer
PwmOut alarmBuzzer(D5);

// Define the LCD object
I2C lcdI2C(D14, D15);
DFRobot_RGBLCD1602 lcd(&lcdI2C);

// buffered serial for debugging
BufferedSerial pc(USBTX, USBRX);

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
    /*
    // Define variables
    int screenNumber = 0;       // you can change this if you want to start on another screen
    int maxScreenNumber = 4;    // max screen should probably be a global variable outside of main
    */
    // initilization
    lcd.init();
    lcd.setRGB(0, 0, 255);  // Set the LCD background color to blue
    set_time(1046673700);   // set RTC to the birth of Albert
    time_t unixtime = time(NULL);

    // Define the alarm screen object
    AlarmScreen alarmScreen(changeScreenLeft, changeScreenRight);
    ScreenHandler screenHandler(0,3,0,2);

    //Button handler object
    ButtonHandler buttonHandler(leftButton, middleButton, rightButton, specialButton, alarmScreen, screenHandler);

    while (true) {
        switch (buttonHandler.getCurrentState()) {
        case ScreenState::ALARM_SCREEN_VIEW: {
            // screen
            alarmScreen.displayAlarmScreen(lcd);
            break;
        }
        
        case ScreenState::TEMP_HUMIDITY_SCREEN: {
            // Bind buttons to news screen

            // Test screen hardcoded
            lcd.display();
            lcd.printf("Temphum");
            ThisThread::sleep_for(100ms);
            lcd.clear();
            break;
        }
        case ScreenState::WEATHER_SCREEN: {
            // test screen hardcoded
            lcd.display();
            lcd.printf("Weather");
            ThisThread::sleep_for(100ms);
            lcd.clear();
            break;
        }
        case ScreenState::NEWS_SCREEN: {
            // test screen hardcoded
            lcd.display();
            lcd.printf("News");
            ThisThread::sleep_for(100ms);
            lcd.clear();
            break;
        }
        default:
            lcd.display();
            lcd.printf("Default is being run");
            ThisThread::sleep_for(100ms);
            lcd.clear();
            break;
        }
    }
}
