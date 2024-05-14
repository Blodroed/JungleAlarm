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
    alarmTime.tm_hour = 8;
    alarmTime.tm_min = 0;
    stateOfSettingAlarm = SET_ALARM_HOUR1;
};

void AlarmScreen::setAlarmTime(const int hour, const int minutes) {
    // Set the alarm time
    alarmTime.tm_hour = hour;
    alarmTime.tm_min = minutes;
}

void AlarmScreen::checkAlarmTime() {
    // initialized snooze time to 5 minutes
    int snoozeMinutes = 0;

    // get the current time and converting it to a struct
    time_t seconds = time(NULL);
    struct tm* now = localtime(&seconds);

    // muteTime is the time the alarm will be muted
    struct tm* muteTime = localtime(&seconds);

    // set mute time to be equal to the alarm time + 10 minutes with some edge cases
    if (alarmTime.tm_hour == 23 && alarmTime.tm_min >= 50) {
        if (muteTime->tm_yday == 365) {
            muteTime->tm_yday = 0;
        } else {
            muteTime->tm_yday = muteTime->tm_yday + 1;
        }
        muteTime->tm_hour = 0;
        muteTime->tm_min = alarmTime.tm_min - 50;
    } else if (alarmTime.tm_min >= 50) {
        muteTime->tm_hour = alarmTime.tm_hour + 1;
        muteTime->tm_min = alarmTime.tm_min - 50;
    } else  {
        muteTime->tm_min = alarmTime.tm_min + 10;
    }

    // if alarm is snoozed we should add 5 minutes to the alarm time
    struct tm* adjustedMutetime = muteTime;
    if (alarmSnoozed) {
        snoozeMinutes = 5;
        if (muteTime->tm_min + snoozeMinutes >= 60) {       // edge casing if the snooze time is more than 60 minutes
            if (muteTime->tm_hour == 23) {
                adjustedMutetime->tm_hour = 0;
            } else {
                adjustedMutetime->tm_hour = muteTime->tm_hour + 1;
            }
            adjustedMutetime->tm_min = muteTime->tm_min + snoozeMinutes - 60;
        } else {
            adjustedMutetime->tm_min += snoozeMinutes;
        }
    }

    // setting the alarm off if the alarm time is equal to the current time
    // and adding snooze time to the alarm time if applicable
    // Long if statement short: 
    // alarmTime.tm_hour <= now->tm_hour <= adjustedMutetime->tm_hour
    // alarmTime.tm_min <= now->tm_min <= adjustedMutetime->tm_min
    if (alarmTime.tm_hour <= now->tm_hour && now->tm_hour <= adjustedMutetime->tm_hour) {
        if (alarmTime.tm_min <= now->tm_min && now->tm_min <= adjustedMutetime->tm_min) {
            alarmActive = true;
        }
    } else {
        alarmActive = false;
    }
    ThisThread::sleep_for(1000ms);
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

void AlarmScreen::disableAlarm() {
    // Disable the alarm
    // here we should update the bool alarmOn to false and stop the alarm thread
}

void AlarmScreen::enableAlarm() {
    // Enable the alarm
    // here we should update the bool alarmOn to true and start the alarm thread
}

void AlarmScreen::snoozeAlarm() {
    // Snooze the alarm
    // here we should update the bool alarmSnoozed to true and add 5 minutes to the alarm time
}

void AlarmScreen::alarmTrigger() {
    // Trigger the alarm
    // here we should start the alarm thread and play the alarm sound

    // ==== ChatGPT thread Start suggestion ====
    /*
     * // Define a boolean variable to keep track of whether the alarm is set
     * bool isAlarmSet = false;
     * 
     * // Define the alarm check thread
     * Thread alarmCheckThread;

     * while (true) {
     *     switch (buttonHandler.getCurrentState()) {
     *     case ScreenState::ALARM_SCREEN_VIEW: {
     *         // screen
     *         if(buttonHandler.getCurrentSubState() == SubScreenState::NO_STATE) {
     *             alarmScreen.displayAlarmScreen(lcd);
     *         } else if(buttonHandler.getCurrentSubState() == SubScreenState::SET_ALARM_SCREEN) {
     *             alarmScreen.displaySetAlarmScreen(lcd);
     * 
     *             // Set the alarm
     *             isAlarmSet = true;
     * 
     *             // Create the alarm check thread if it's not already running
     *             if (!alarmCheckThread.get_state()) {
     *                 alarmCheckThread.start(callback(&alarmCheckFunction));
     *             }
     *         }
     *  
     *         break;
     *     }
     *     // Rest of your code...
     * }
     */
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
    if (!isAlarmSet) {
    lcd.printf("ALARM NOT SET");
    }
    
    ThisThread::sleep_for(200ms);
}

void AlarmScreen::threadStart() {
    alarmThread.start(callback(this, &AlarmScreen::checkAlarmTime));
}

