//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>

// Constructor
AlarmScreen::AlarmScreen(void (*changeScreenLeft)(int screenNumber, int maxScreenNumber), void (*changeScreenRight)(int screenNumber, int maxScreenNumber)) {
    // Constructor
    this->changeScreenLeft = changeScreenLeft;
    this->changeScreenRight = changeScreenRight;
    
}

void AlarmScreen::setAlarmTime() {
    // Set the alarm time
}

void AlarmScreen::setAlarmScreen() {
    // Set the alarm screen

}

void AlarmScreen::muteAlarm() {
    // Mute the alarm
    // here we should update the bool alarmActive to false and stop the alarm thread
}

void AlarmScreen::displayAlarmScreen(DFRobot_RGBLCD1602 &lcd) {
    // get seconds since epoch
    time_t seconds = rtc_read();

    // get the local time
    struct tm* now = localtime(&seconds);

    // format the time correctly
    char buffer[42] = {0};
    strftime(buffer, 32, "%a %d %b %H:%M", localtime(&seconds));

    // Display the time on the LCD
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);  // Set cursor to line 1 (top line)
    lcd.printf(buffer);  // Print the formatted time

    // Display the alarm screen
    lcd.setCursor(0, 2);
    lcd.printf("ALARM");

    ThisThread::sleep_for(200ms);
}