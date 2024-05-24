/**
 * @file main.cpp
 * @author Albert Salvesen-Orø
 * @author Aristidis Akritidis
 */

#include "mbed.h"
#include "DFRobot_RGBLCD1602.h"
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
InterruptIn leftButton(A5, PullUp); 
InterruptIn middleButton(D2, PullUp);
InterruptIn rightButton(A3, PullUp);
InterruptIn specialButton(A2, PullUp);

// threads
Thread alarmThread;

int main()
{
    // initilization
    lcd.init();
    lcd.setRGB(150, 50, 255);  // Set the LCD background color to blue
    set_time(1046674910);   // set RTC to the birth of Albert
    time_t unixtime = time(NULL);
    alarmBuzzer.write(0.0f);

    //Button handler object
    AlarmScreen alarmScreen(alarmBuzzer);
    Temphum tempHumScreen;
    ButtonHandler buttonHandler(leftButton, middleButton, rightButton, specialButton, alarmScreen, lcd, tempHumScreen);

    // initilize the alarmThread
    alarmThread.start(callback(&alarmScreen, &AlarmScreen::checkAlarmTime));

    while (true) {
        switch (buttonHandler.getCurrentState()) {
        case ScreenState::ALARM_SCREEN_VIEW: {
            // screen
            if(buttonHandler.getCurrentSubState() == SubScreenState::NO_STATE) {
                alarmScreen.displayAlarmScreen(lcd);
            } else if(buttonHandler.getCurrentSubState() == SubScreenState::SET_ALARM_SCREEN) {
                alarmScreen.displaySetAlarmScreen(lcd);
            }
            break;
        }
        
        case ScreenState::TEMP_HUMIDITY_SCREEN: {
            // Bind buttons to news screen
            // Test screen hardcoded
            tempHumScreen.displayTempHumScreen(lcd);
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
