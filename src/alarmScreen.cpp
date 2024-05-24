/**
 * @file alarmscreen.cpp
 * @author Albert Salvesen-Orø
 * @author Aristidis Akritidis
 */

#include "../include/alarmScreen.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "../include/buttonHandler.h"

#include <ctime>
#include <iterator>


UnbufferedSerial pc1(USBTX, USBRX);

// Constructor
AlarmScreen::AlarmScreen(PwmOut &buzzer)
            : alarmBuzzer(buzzer),
              stateOfSettingAlarm(SettingAlarmState::SET_ALARM_HOUR1){
    alarmEnabled = true;
    alarmSnoozed = 0;
    alarmMuted = false;
    alarmActive = false;
    isAlarmSet = true;
    alarmTime.tm_hour = 8;
    alarmTime.tm_min = 2;
    setHour1 = 0;
    setHour2 = 8;
    setMin1 = 0;
    setMin2 = 2;

    // TODO: remove change to right statements before sending

    playSoundThread.start(callback(this, &AlarmScreen::playAlarmSound));
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
    while(true) {
        if (!alarmEnabled) {
            ThisThread::sleep_for(1000ms);
            continue;
        }
        
        // initialized snooze time to 5 minutes
        int snoozeMinutes = 1;

        // get the current time and converting it to a struct
        time_t seconds = time(NULL);
        struct tm* now = localtime(&seconds);

        // Calculate mute time as 10 minutes after the alarm time
        struct tm muteTime = alarmTime;
        muteTime.tm_min += 3;
        if (muteTime.tm_min >= 60) {
            muteTime.tm_min -= 60;
            muteTime.tm_hour += 1;
            if (muteTime.tm_hour >= 24) {
                muteTime.tm_hour -= 24;
                muteTime.tm_yday += 1;
            }
        }

        // Adjust mute time if the alarm is snoozed
        if (alarmSnoozed >= 1 && alarmSnoozedInstance) {
            muteTime.tm_min += snoozeMinutes;
            alarmTime.tm_min += snoozeMinutes;
            alarmSnoozedInstance = false;
            if (muteTime.tm_min >= 60) {
                muteTime.tm_min -= 60;
                muteTime.tm_hour += 1;
                if (muteTime.tm_hour >= 24) {
                    muteTime.tm_hour -= 24;
                    muteTime.tm_yday += 1;
                }
            }
            if (alarmTime.tm_min >= 60) {
                alarmTime.tm_min -= 60;
                alarmTime.tm_hour += 1;
                if (alarmTime.tm_hour >= 24) {
                    alarmTime.tm_hour -= 24;
                    alarmTime.tm_yday += 1;
                }
            }
        }

        // Check if the current time is within the alarm window
        bool isWithinAlarmWindow = false;
        if (now->tm_hour <= muteTime.tm_hour && now->tm_min <= muteTime.tm_min) {
            if (now->tm_hour >= alarmTime.tm_hour && now->tm_min >= alarmTime.tm_min) {
                isWithinAlarmWindow = true;
            } else {
                alarmMuted = true;
            }
        } else {
            alarmMuted = true;
            muteAlarm();
        }

        // Activate or deactivate the alarm based on the current time
        if (isWithinAlarmWindow && !alarmMuted) {
            if (!alarmActive && !alarmMuted) {
                alarmTrigger();
            }
            alarmActive = true;
        } else if (alarmMuted && !isWithinAlarmWindow) {
            alarmActive = false;
            alarmMuted = false;
        }
        if (alarmActive) {
            ThisThread::sleep_for(500ms);
        } else {
            ThisThread::sleep_for(1000ms);
        }
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
}

void AlarmScreen::muteAlarm() {
    // Mute the alarm
    // here we should update the bool alarmActive to false and stop the alarm thread
    alarmSnoozed = 0;
    alarmMuted = true;
    alarmActive = false;

    // reset alarmtime to stored alarm time
    convertAlarmTimeToStruct();

    // turn off the buzzer
    alarmBuzzer.write(0.0);
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
    alarmActive = false;
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
        alarmSnoozedInstance = true;
        alarmSnoozed++;
        alarmActive = false; // this should also stop the buzzer alarm
    }
}

void AlarmScreen::alarmTrigger() {
    // Trigger the alarm
    // set the alarm to active
    alarmActive = true;
}

void AlarmScreen::displayAlarmScreen(DFRobot_RGBLCD1602 &lcd) {
    // get seconds since epoch
    time_t seconds = rtc_read();

    // get the local time
    struct tm* now = localtime(&seconds);

    // Create separate buffers for hours and minutes
    char hourBuffer[3];
    char minBuffer[3];

    // Format the hours and minutes with zero padding
    sprintf(hourBuffer, "%02d", now->tm_hour);
    sprintf(minBuffer, "%02d", now->tm_min);

    // Create the final buffer for the LCD
    char bufferLCD[42] = {0};

    // Format the date with strftime
    char dateBuffer[20];
    strftime(dateBuffer, sizeof(dateBuffer), "%a %d %b", now);

    // Combine the date and time into the final string
    sprintf(bufferLCD, "%s %s:%s", dateBuffer, hourBuffer, minBuffer);

    // Display the time on the LCD
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);  // Set cursor to line 1 (top line)
    lcd.printf(bufferLCD);  // Print the formatted time

    // Display the alarm screen
    lcd.setCursor(0, 1);
    if (!isAlarmSet) {
        lcd.printf("ALARM NOT SET");
    } else if (isAlarmSet && alarmSnoozed >= 1 && !alarmActive) {
        lcd.printf("Alarm (s)  %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (isAlarmSet && alarmEnabled && !alarmActive) {
        lcd.printf("Alarm      %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (isAlarmSet && alarmActive && !alarmMuted) {
        lcd.printf("ALARM (A)  %d%d:%d%d", setHour1, setHour2, setMin1, setMin2);
    } else if (!alarmEnabled) {
        lcd.printf("----------------");
    }

    /*
    // char buffer to computer of the values in alarmscreen
    char buffer[256];
    sprintf(buffer, "----------------\n");
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "alarmEnabled: %d\n", alarmEnabled);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "alarmSnoozed: %d\n", alarmSnoozed);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "alarmMuted: %d\n", alarmMuted);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "alarmActive: %d, ", alarmActive);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "alarmTime: %d:%d, ", alarmTime.tm_hour, alarmTime.tm_min);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "isAlarmSet: %d, ", isAlarmSet);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "stateOfSettingAlarm: %d\n", static_cast<int>(stateOfSettingAlarm));
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "setHour1: %d, ", setHour1);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "setHour2: %d, ", setHour2);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "setMin1: %d, ", setMin1);
    pc1.write(buffer, strlen(buffer));

    sprintf(buffer, "setMin2: %d\n", setMin2);
    pc1.write(buffer, strlen(buffer));*/

    ThisThread::sleep_for(300ms);
}

SettingAlarmState AlarmScreen::changeTimeState() {
    int nextTimeState = static_cast<int>(stateOfSettingAlarm) + 1;
    if(nextTimeState>5) {
        nextTimeState = 5;
    }
    stateOfSettingAlarm = static_cast<SettingAlarmState>(nextTimeState);
    return stateOfSettingAlarm;
}

// thread function for playing the alarm
void AlarmScreen::playAlarmSound() {
    while (true) {
        if (alarmActive && !alarmMuted) {
            alarmBuzzer.period(1.0/440.0);
            alarmBuzzer.write(0.5);
            ThisThread::sleep_for(500ms);
            alarmBuzzer.write(0.0);
            ThisThread::sleep_for(500ms);
        } else {
            alarmBuzzer.write(0.0);
            ThisThread::sleep_for(1000ms);
        }
    }
}

// mutex locking
void AlarmScreen::lockMutex() {
    // lock the mutex
    alarmMutex.lock();
}
void AlarmScreen::unlockMutex() {
    // unlock the mutex
    alarmMutex.unlock();
}

bool AlarmScreen::getAlarmActive() {
    return alarmActive;
}

bool AlarmScreen::getAlarmMuted() {
    return alarmMuted;
}

int AlarmScreen::getAlarmSnoozed() {
    return alarmSnoozed;
}
