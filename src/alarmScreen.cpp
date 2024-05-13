//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>
#include <iterator>

// Constructor
AlarmScreen::AlarmScreen() : stateOfSettingAlarm(SettingAlarmState::SET_ALARM_HOUR1){};

void AlarmScreen::setAlarmTime() {
    switch (stateOfSettingAlarm) {
        case SettingAlarmState::SET_ALARM_HOUR1: 
            setHour1 = (setHour1+1)%3;
            break;
        case SettingAlarmState::SET_ALARM_HOUR2: 
            if(setHour1==2) {
                setHour2 = (setHour2+1)%5;
                break;
            }
            setHour2 = (setHour2+1)%10;
            break;
        case SettingAlarmState::SET_ALARM_MINUTE1: 
            setMin1 = (setMin1+1)%6;
            break;
        case SettingAlarmState::SET_ALARM_MINUTE2:
            setMin2 = (setMin2+1)%10;
            break;
        default:
            break;
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
    lcd.printf("Time: %d%d:%d%d",setHour1,setHour2,setMin1,setMin2);
    lcd.setCursor(0, 1);
    
    char message[] = " + | accept | - ";
    lcd.printf(message);
    ThisThread::sleep_for(300ms);
    /*
    for (int i = 0; i < strlen(message)-15; i++) {
        lcd.scrollDisplayLeft();
        ThisThread::sleep_for(300ms);
    }
    */
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

SettingAlarmState AlarmScreen::changeTimeState() {
    int nextTimeState = static_cast<int>(stateOfSettingAlarm) + 1;
    if(nextTimeState>5) {
        nextTimeState = 5;
    }
    stateOfSettingAlarm = static_cast<SettingAlarmState>(nextTimeState);
    return stateOfSettingAlarm;
}