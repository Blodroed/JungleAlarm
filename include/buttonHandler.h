#include "mbed.h"
#include "libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "include/alarmScreen.h"
#include "screenHandler.h"

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

    ButtonHandler(InterruptIn& leftButton_, InterruptIn& middleButton_, InterruptIn& rightButton_, InterruptIn& specialButton_, AlarmScreen &alarmScreen_, ScreenHandler screenHandler_)
            : leftButton(leftButton_), middleButton(middleButton_), rightButton(rightButton_), specialButton(specialButton_), alarmScreen(alarmScreen_), screenHandler(screenHandler_), currentState(ScreenState::ALARM_SCREEN_VIEW), currentSubState(SubScreenState::NO_STATE) {

        leftButton.rise(callback(this,&ButtonHandler::handleLeftButton));
        middleButton.rise(callback(this,&ButtonHandler::handleMiddleButton));
        rightButton.rise(callback(this,&ButtonHandler::handleRightButton));
        specialButton.rise(callback(this,&ButtonHandler::handleSpecialButton));
    }

    void handleLeftButton();

    void handleMiddleButton();

    void handleRightButton();

    void handleSpecialButton();

    void changeStateLeft();
    void changeStateRight();

    void changeSubState();

private:
    ScreenState currentState;
    SubScreenState currentSubState;
    AlarmScreen &alarmScreen;
    //WeatherScreen &weatherScreen;
    //TempHumidityScreen &tempHumidityScreen;
    //NewsScreen &newsScreen;
    ScreenHandler screenHandler;

    InterruptIn &leftButton;
    InterruptIn &middleButton;
    InterruptIn &rightButton;
    InterruptIn &specialButton;
};