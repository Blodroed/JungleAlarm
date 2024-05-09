//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

// Constructor
AlarmScreen::AlarmScreen(void (*changeScreenLeft)(int screenNumber, int maxScreenNumber), void (*changeScreenRight)(int screenNumber, int maxScreenNumber)) {
    // Constructor
    this->changeScreenLeft = changeScreenLeft;
    this->changeScreenRight = changeScreenRight;
    stateOfAlarm = ALARM_MENU;
    
}

void AlarmScreen::setAlarmTime() {
    // Set the alarm time
}

void AlarmScreen::setAlarmScreen() {
    // Set the alarm screen
}

void AlarmScreen::displayAlarmScreen(DFRobot_RGBLCD1602 &lcd) {
    // Display the alarm screen
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 2);
    lcd.printf("ALARM");
    ThisThread::sleep_for(100ms);
    
}

void AlarmScreen::leftButtonPressed() {
    // Handle the left button press
}

void AlarmScreen::middleButtonPressed() {
    // Handle the middle button press
}

void AlarmScreen::rightButtonPressed(int currentScreenNumberm, int maxScreenNumber) {
    // Handle the right button press
    switch (stateOfAlarm) {
        case ALARM_MENU:
            changeScreenRight(currentScreenNumber, maxScreenNumber);

            break;
        case SET_ALARM:
            // do something
            break;
        default:
            break;

    }

}

void AlarmScreen::specialButtonPressed() {
    // Handle the special button press
}