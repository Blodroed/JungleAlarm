//
// Created by Albert on 30.04.2024.
//

#ifndef EXAM_PROJECT_ALARMSCREEN_H
#define EXAM_PROJECT_ALARMSCREEN_H

#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <ctime>

class AlarmScreen {
public:
    enum alarmState {
        ALARM_MENU,
        SET_ALARM
    };

    enum settingAlarmState {
        SET_ALARM_HOUR1,
        SET_ALARM_HOUR2,
        SET_ALARM_MINUTE1,
        SET_ALARM_MINUTE2,
        ACCEPT
    };

    // Constructor
    AlarmScreen(void (*changeScreenLeft)(int screenNumber, int maxScreenNumber), void (*changeScreenRight)(int screenNumber, int maxScreenNumber));

    void setAlarmTime();
    void setAlarmScreen();
    void displayAlarmScreen(DFRobot_RGBLCD1602 &lcd);

    // button handling
    void leftButtonPressed();
    void middleButtonPressed();
    void rightButtonPressed(alarmState state);
    void specialButtonPressed();
    

private:
    bool alarmOn;
    bool alarmActive;
    

    void (*changeScreenLeft)(int screenNumber, int maxScreenNumber);
    void (*changeScreenRight)(int screenNumber, int maxScreenNumber);
};


#endif //EXAM_PROJECT_ALARMSCREEN_H
