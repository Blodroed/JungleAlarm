//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

// Constructor
AlarmScreen::AlarmScreen(void (*changeScreenLeft)(int screenNumber, int maxScreenNumber), void (*changeScreenRight)(int screenNumber, int maxScreenNumber)) {
    // Constructor
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
    }
}

void AlarmScreen::specialButtonPressed() {
    // Handle the special button press
}