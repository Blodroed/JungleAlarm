//
// Created by Albert on 30.04.2024.
//

#ifndef EXAM_PROJECT_ALARMSCREEN_H
#define EXAM_PROJECT_ALARMSCREEN_H

#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>

class AlarmScreen {
public:
    enum settingAlarmState {
        SET_ALARM_HOUR1,
        SET_ALARM_HOUR2,
        SET_ALARM_MINUTE1,
        SET_ALARM_MINUTE2,
        ACCEPT
    };

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

    void setAlarmTime();
    
private:
    bool alarmOn;
    bool alarmSnoozed;
    bool alarmMuted;
    bool alarmActive;

    int alarmHour;
    int alarmMinute;

    settingAlarmState stateOfSettingAlarm;
};


#endif //EXAM_PROJECT_ALARMSCREEN_H
