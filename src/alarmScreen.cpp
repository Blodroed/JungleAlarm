//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>

// Constructor
AlarmScreen::AlarmScreen(){
    alarmOn = false;
    alarmSnoozed = false;
    alarmMuted = false;
    alarmActive = false;
    alarmHour = 12;
    alarmMinute = 15;
    stateOfSettingAlarm = SET_ALARM_HOUR1;
};

void AlarmScreen::setAlarmTime() {
    // Set the alarm time
}

void AlarmScreen::checkAlarmTime() {
    // Check if the alarm time is equal to or 5 min above the current time
    time_t seconds = time(NULL);
    time_t stopTime = seconds + (10 * 60);  // adding 10 minutes to default stoptime


    if ((now->tm_hour == alarmHour && now->tm_min >= alarmMinute) && (now->tm_hour == alarmHour && now->tm_min <= alarmMinute + 10)) {
        // Alarm is active
        alarmActive = true;
    }
    else {
        // Alarm is not active
        alarmActive = false;
    }
}

void AlarmScreen::displaySetAlarmScreen(DFRobot_RGBLCD1602 &lcd) {
    // the screen where the user can set the alarm using
    // left button will decrease the value, and right button will increase the value
    // middle button will change the state to the next state and on accept it will
    // set the alarm time

    //There is a MAX number of characters per line of 40. 
    //Any more than that and the rows start to bleed into eachother and override eachother
    //Limit any message on a given line to 40 characters or less
    
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);  // Set cursor to line 1 (top line)
    lcd.printf("Set Alarm Time");
    lcd.setCursor(0, 1);
    
    char message[] = "Right button: add, left button: remove";
    lcd.printf(message);
    ThisThread::sleep_for(1000ms);
    for (int i = 0; i < strlen(message)-15; i++) {
        lcd.scrollDisplayLeft();
        ThisThread::sleep_for(300ms);
    }
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
    strftime(buffer, 42, "%a %d %b %R", localtime(&seconds));

    // Display the time on the LCD
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);  // Set cursor to line 1 (top line)
    lcd.printf(buffer);  // Print the formatted time

    // Display the alarm screen
    lcd.setCursor(0, 1);
    lcd.printf("ALARM");

    ThisThread::sleep_for(200ms);
}