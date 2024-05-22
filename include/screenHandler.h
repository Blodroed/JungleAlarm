#ifndef JUNGLEALARM_SCREEN_HANDLER_H
#define JUNGLEALARM_SCREEN_HANDLER_H

#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <algorithm>
class ScreenHandler {

public:
    ScreenHandler(int currentScreenNumber, int maxScreenNumber, int currentSubScreenNumber, int maxSubScreenNumber);

    // Display Alarm
    void displayAlarm(DFRobot_RGBLCD1602 &lcd);

    // change main screen
    void changeScreenRight();
    void changeScreenLeft();

    // change subscreen
    void changeSubScreen();

    // getters
    int getCurrentSubScreenNumber();
    int getCurrentScreenNumber();
    int getMaxScreenNumber();

private:
    int maxScreenNumber;
    int currentScreenNumber;

    int currentSubScreenNumber;
    int maxSubScreenNumber;
};

#endif // JUNGLEALARM_SCREEN_HANDLER_H