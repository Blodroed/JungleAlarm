/**
 * @file buttonHandler.h
 * @author Albert Salvesen-Orø
 * @author Aristidis Akritidis
 */

#include "mbed.h"
#include "libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"
#include "include/alarmScreen.h"
#include "mbed_events.h"
#include "tempHum.h"

#include <algorithm>
#include <ctime>

class AlarmScreen;

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

    ButtonHandler(InterruptIn& leftButton_, InterruptIn& middleButton_, InterruptIn& rightButton_, InterruptIn& specialButton_, AlarmScreen &alarmScreen_, DFRobot_RGBLCD1602 &lcd_, Temphum &temphumScreen_)
            : leftButton(leftButton_), middleButton(middleButton_), rightButton(rightButton_), specialButton(specialButton_), alarmScreen(alarmScreen_), tempHumidityScreen(temphumScreen_), currentState(ScreenState::ALARM_SCREEN_VIEW), currentSubState(SubScreenState::NO_STATE), lcd(lcd_) {

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
    void changeToAlarmScreen();

    void changeTimeState();

    // eventqueue for interrupts
    EventQueue eventQueue;

    // getters
    ScreenState getCurrentState() { return currentState; }
    SubScreenState getCurrentSubState() { return currentSubState; }

private:
    ScreenState currentState;
    SubScreenState currentSubState;
    SettingAlarmState currentTimeState; // the time digit in the time setting screen the user is changing (hour1, hour2, min1, min2)
    //SettingAlarmState currentSettingAlarmState;

    AlarmScreen &alarmScreen;
    //WeatherScreen &weatherScreen;
    Temphum &tempHumidityScreen;
    //NewsScreen &newsScreen;

    InterruptIn &leftButton;
    InterruptIn &middleButton;
    InterruptIn &rightButton;
    InterruptIn &specialButton;

    //Variables for debouncing buttons

    DFRobot_RGBLCD1602 &lcd;
};