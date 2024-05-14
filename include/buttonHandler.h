#include "mbed.h"
#include "libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "include/alarmScreen.h"
#include "screenHandler.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

#include <algorithm>
#include <ctime>


enum class ScreenState {
    ALARM_SCREEN_VIEW,
    WEATHER_SCREEN,
    TEMP_HUMIDITY_SCREEN,
    NEWS_SCREEN
};

enum class SubScreenState {
    NO_STATE,
    SET_ALARM_SCREEN,
    SET_IP_ADDRESS
};

class ButtonHandler {
public:

    ButtonHandler(InterruptIn& leftButton_, InterruptIn& middleButton_, InterruptIn& rightButton_, InterruptIn& specialButton_, AlarmScreen &alarmScreen_, ScreenHandler screenHandler_, DFRobot_RGBLCD1602 &lcd_)
            : leftButton(leftButton_), middleButton(middleButton_), rightButton(rightButton_), specialButton(specialButton_), alarmScreen(alarmScreen_), screenHandler(screenHandler_), currentState(ScreenState::ALARM_SCREEN_VIEW), currentSubState(SubScreenState::NO_STATE), lcd(lcd_) {

        leftButton.rise(callback(this,&ButtonHandler::handleLeftButton));
        middleButton.rise(callback(this,&ButtonHandler::handleMiddleButton));
        rightButton.rise(callback(this,&ButtonHandler::handleRightButton));
        specialButton.rise(callback(this,&ButtonHandler::handleSpecialButton));
    }

    // handling button presses
    void handleLeftButton();
    void handleMiddleButton();
    void handleRightButton();
    void handleSpecialButton();

    // changing screen states
    void changeStateLeft();
    void changeStateRight();
    void changeSubState();

    void changeTimeState();

    // getters
    ScreenState getCurrentState() { return currentState; }
    SubScreenState getCurrentSubState() { return currentSubState; }

private:
    ScreenState currentState;
    SubScreenState currentSubState;
    SettingAlarmState currentTimeState;
    //which digit in the time setting screen the user is changing (hour1, hour2, min1, min2)
    //SettingAlarmState currentSettingAlarmState;

    AlarmScreen &alarmScreen;
    //WeatherScreen &weatherScreen;
    //TempHumidityScreen &tempHumidityScreen;
    //NewsScreen &newsScreen;
    ScreenHandler screenHandler;

    InterruptIn &leftButton;
    InterruptIn &middleButton;
    InterruptIn &rightButton;
    InterruptIn &specialButton;

    //Variables for debouncing buttons

    DFRobot_RGBLCD1602 &lcd;
};