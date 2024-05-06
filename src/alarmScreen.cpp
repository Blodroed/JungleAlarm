//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

// Constructor ==========================================
AlarmScreen::AlarmScreen(void (*changeScreenLeft)(int screenNumber, int maxScreenNumber), void (*changeScreenRight)(int screenNumber, int maxScreenNumber)) {
    // Constructor
    /*  Github copilot help:
        alarmThread.start(callback(this, &AlarmScreen::checkAlarm));
    */
}

// Alarm handling ========================================

/* void AlarmScreen::checkAlarm() {
        while (true) {
            // Get the current time
            time_t currentTime = time(NULL);

            // Check if the current time is equal to the alarm time
            if (currentTime == alarmTime) {
                // If so, set off the alarm
                setOffAlarm();
            }

            // Wait for a second before checking again
            ThisThread::sleep_for(1s);
        }
    }
*/

void AlarmScreen::setAlarmTime() {
    // The fucntion to set the alarm time with values from setAlarmScreen
}

void AlarmScreen::setAlarmScreen() {
    // Function to set the alarm time with the buttons and prompts from the user
}

void AlarmScreen::displayAlarmScreen(DFRobot_RGBLCD1602 &lcd) {
    // Display the alarm screen
    lcd.display();
    lcd.setCursor(0, 2);
    lcd.printf("ALARM");
}

void AlarmScreen::leftButtonPressed() {
    // Handle the left button press
}

void AlarmScreen::middleButtonPressed() {
    // Handle the middle button press
}

void AlarmScreen::rightButtonPressed() {
    // Handle the right button press
    switch (alarmState) {
    case ALARM_MENU:
        changeScreenRight();
        break;
    default:
        break;
    }
}

void AlarmScreen::specialButtonPressed() {
    // Handle the special button press
}