/**
 * @file alarmscreen.h
 * @author Albert Salvesen-Orø
 * @author Aristidis Akritidis
 */

#ifndef EXAM_PROJECT_ALARMSCREEN_H
#define EXAM_PROJECT_ALARMSCREEN_H

#include "DFRobot_RGBLCD1602.h"

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
    AlarmScreen(PwmOut &buzzer);

    // check alarmtime to rtctime
    void checkAlarmTime();

    // alarmdisplay functions
    void displaySetAlarmScreen(DFRobot_RGBLCD1602 &lcd);
    void displayAlarmScreen(DFRobot_RGBLCD1602 &lcd);

    // control functions for alarm
    void muteAlarm();
    void disableAlarm();
    void enableAlarm();
    void alarmSwitch();  // enable or disable alarm with a button
    void snoozeAlarm();
    void alarmTrigger();

    // set alarm time and converting it to a struct
    void setAlarmTimeMore();
    void setAlarmTimeLess();
    void convertAlarmTimeToStruct();

    // mutex locking
    void lockMutex();
    void unlockMutex();

    //Getters for alarm states
    bool getAlarmActive();
    bool getAlarmMuted();
    int getAlarmSnoozed();

    // alarmsound
    void playAlarmSound();

    //The thread managing the alarm and shit
    Thread buzzerRinging;

    // variables for setAlarmscreen states
    SettingAlarmState changeTimeState();
    SettingAlarmState stateOfSettingAlarm;

    // variables for setting alarm
    int setHour1 = 0;
    int setHour2 = 0;
    int setMin1 = 0;
    int setMin2 = 0;

private:
    bool alarmSnoozedInstance;
    bool alarmEnabled;      // enabled or Disabled alarm
    int alarmSnoozed;       // incremented snooze timer
    bool alarmMuted;        // muted or not
    bool alarmActive;       // if the alarm is ringing
    bool isAlarmSet;        // if the alarm time is set

    PwmOut &alarmBuzzer;

    //alarm time as struct
    struct tm alarmTime;

    // play sound thread
    Thread playSoundThread;

    // mutex for mutex exceptions
    Mutex alarmMutex;

};


#endif //EXAM_PROJECT_ALARMSCREEN_H
