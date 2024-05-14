//
// Created by Albert on 30.04.2024.
//

#ifndef EXAM_PROJECT_ALARMSCREEN_H
#define EXAM_PROJECT_ALARMSCREEN_H

#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>

enum class SettingAlarmState {
        SET_ALARM_HOUR1,
        SET_ALARM_HOUR2,
        SET_ALARM_MINUTE1,
        SET_ALARM_MINUTE2,
        ACCEPT
};

class AlarmScreen {
public:
    // Constructor
    AlarmScreen();

    // check alarmtime to rtctime
    void checkAlarmTime();

    // alarmdisplay functions
    void displaySetAlarmScreen(DFRobot_RGBLCD1602 &lcd);
    void displayAlarmScreen(DFRobot_RGBLCD1602 &lcd);

    // control functions for alarm
    void muteAlarm();
    void disableAlarm();
    void enableAlarm();
    void snoozeAlarm();
    void alarmTrigger();

    void setAlarmTime();

    void threadStart();

    //The thread managing the alarm and shit
    Thread alarmThread;

    SettingAlarmState changeTimeState();

    SettingAlarmState stateOfSettingAlarm;

    int alarmHour;
    int alarmMinute;
    
    int setHour1 = 0;
    int setHour2 = 0;
    int setMin1 = 0;
    int setMin2 = 0;

private:
    bool alarmOn;
    int alarmSnoozed;
    bool alarmMuted;
    bool alarmActive;
    bool isAlarmSet;

    //alarm time as struct
    struct tm alarmTime;
};


#endif //EXAM_PROJECT_ALARMSCREEN_H
