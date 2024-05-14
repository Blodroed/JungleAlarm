//
// Created by Albert on 30.04.2024.
//

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>
#include <iterator>

// Constructor
AlarmScreen::AlarmScreen() : stateOfSettingAlarm(SettingAlarmState::SET_ALARM_HOUR1){
    alarmEnabled = false;
    alarmSnoozed = 0;
    alarmMuted = false;
    alarmActive = false;
    alarmTime.tm_hour = 8;
    alarmTime.tm_min = 0;
};

void AlarmScreen::setAlarmTimeMore() {
    // Set the alarm time
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

void AlarmScreen::setAlarmTimeLess() {
    // decrement all alarm time values
    switch (stateOfSettingAlarm) {
        case SettingAlarmState::SET_ALARM_HOUR1:
            setHour1 = (setHour1 - 1) % 3;
            if (setHour1 < 0) {    // modulo works differently in C++ than in math
                setHour1 = 2;
            }
            break;
        case SettingAlarmState::SET_ALARM_HOUR2:
            if (setHour1 == 2) {
                setHour2 = (setHour2 - 1) % 5;
                if (setHour2 < 0) {
                    setHour2 = 4;
                }
                break;
            }
            setHour2 = (setHour2 - 1) % 10;
            if (setHour2 < 0) {
                setHour2 = 9;
            }
            break;
        case SettingAlarmState::SET_ALARM_MINUTE1:
            setMin1 = (setMin1 - 1) % 6;
            if (setMin1 < 0) {
                setMin1 = 5;
            }
            break;
        case SettingAlarmState::SET_ALARM_MINUTE2:
            setMin2 = (setMin2 - 1) % 10;
            if (setMin2 < 0) {
                setMin2 = 9;
            }
            break;
        default:
            break;
    }
}

void AlarmScreen::convertAlarmTimeToStruct() {
    // is only called when the user has accepted the alarm time in buttonHandler
    if (setHour1 == 2 && setHour2 > 3) {    // edge case for 24 hour clock
        setHour1 = 0;
        setHour2 = 0;
    }
    alarmTime.tm_hour = setHour1*10 + setHour2;
    alarmTime.tm_min = setMin1*10 + setMin2;
    isAlarmSet = true;
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
    if (alarmSnoozed >= 1) {
        snoozeMinutes = 5;
        if (muteTime->tm_min + snoozeMinutes >= 60) {       // edge casing if the snooze time is more than 60 minutes
            if (muteTime->tm_hour == 23) {
                if (adjustedMutetime->tm_yday == 365) {
                    adjustedMutetime->tm_yday = 0;
                } else {
                    adjustedMutetime->tm_yday = muteTime->tm_yday + 1;
                }
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
    lcd.printf("Time: %d%d:%d%d",setHour1,setHour2,setMin1,setMin2);
    lcd.setCursor(0, 1);
    
    char message[] = " + | accept | - ";
    lcd.printf(message);
    ThisThread::sleep_for(300ms);
}

void AlarmScreen::muteAlarm() {
    // Mute the alarm
    // here we should update the bool alarmActive to false and stop the alarm thread
    alarmSnoozed = 0;
}

void AlarmScreen::disableAlarm() {
    // Disable the alarm
    // here we should update the bool alarmEnabled to false and stop the alarm thread
    alarmEnabled = false;
    alarmActive = false;
}

void AlarmScreen::enableAlarm() {
    // Enable the alarm
    // here we should update the bool alarmEnabled to true and start the alarm thread
    alarmEnabled = true;
}

void AlarmScreen::alarmSwitch() {
    // Switch the alarm on/off
    // here we should update the bool alarmEnabled to the opposite of what it is
    if (alarmEnabled) {
        disableAlarm();
    } else {
        enableAlarm();
    }
}

void AlarmScreen::snoozeAlarm() {
    // Snooze the alarm
    // here we should update the bool alarmSnoozed to true and add 5 minutes to the alarm time
    if (alarmActive) {
        alarmSnoozed++;
        alarmActive = false; // this should also stop the buzzer alarm
    }
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
    } else if (isAlarmSet && alarmSnoozed >= 1) {
        lcd.printf("Alarm (s)  %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (isAlarmSet && alarmEnabled) {
        lcd.printf("Alarm      %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (isAlarmSet && alarmActive) {
        lcd.printf("ALARM (a)  %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (!alarmEnabled) {
        lcd.printf("----------------");
    }
    
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

void AlarmScreen::threadStart() {
    alarmThread.start(callback(this, &AlarmScreen::checkAlarmTime));
}

