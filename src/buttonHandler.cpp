#include "buttonHandler.h"
#include <cctype>
#include <chrono>
#include <cstdio>
#include <exception>
#include <ratio>
#include <string>

BufferedSerial pc1(USBTX, USBRX);

void ButtonHandler::handleLeftButton() {
    switch(currentSubState) {
        case SubScreenState::NO_STATE: changeStateLeft(); break;
        case SubScreenState::SET_ALARM_SCREEN: alarmScreen.setAlarmTime(); break;
        case SubScreenState::SET_IP_ADDRESS: break;
        default: break;
    }

}

void ButtonHandler::handleMiddleButton() {
    if (currentSubState == SubScreenState::NO_STATE) {
        // DO NOT I REPEAT DO NOT RUN FUNCTIONS INSIDE THIS SWITCH CASE
        switch (currentState) {
            case ScreenState::ALARM_SCREEN_VIEW:
                currentSubState = SubScreenState::SET_ALARM_SCREEN;
                break;
            case ScreenState::WEATHER_SCREEN:
                break;
            case ScreenState::TEMP_HUMIDITY_SCREEN:
                break;
            case ScreenState::NEWS_SCREEN:
                break;
            default:
                break;
        }
        return;
    }
    switch(currentSubState) {
        case SubScreenState::SET_ALARM_SCREEN: 
            changeTimeState();
            if(alarmScreen.stateOfSettingAlarm == SettingAlarmState::ACCEPT) {
                alarmScreen.alarmHour = alarmScreen.setHour1 * 10 + alarmScreen.setHour2;
                alarmScreen.alarmMinute = alarmScreen.setMin1 * 10 + alarmScreen.setMin2;
                alarmScreen.stateOfSettingAlarm = SettingAlarmState::SET_ALARM_HOUR1;
                currentSubState = SubScreenState::NO_STATE;
                
            }
            break;
        case SubScreenState::SET_IP_ADDRESS: break;
        default: break;
    }    
    
}

void ButtonHandler::handleRightButton() {
    switch (currentSubState) {
        case SubScreenState::NO_STATE: changeStateRight(); break;
        case SubScreenState::SET_ALARM_SCREEN: break;
        case SubScreenState::SET_IP_ADDRESS: break;
        default: break;
    }
    
}

void ButtonHandler::handleSpecialButton() {
    switch(currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: break;
        case ScreenState::WEATHER_SCREEN: break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: break;
        case ScreenState::NEWS_SCREEN: break;
        default: break;
        }
}

void ButtonHandler::changeStateLeft() {
    switch (currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: currentState = ScreenState::NEWS_SCREEN; break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: currentState = ScreenState::ALARM_SCREEN_VIEW; break;
        case ScreenState::WEATHER_SCREEN: currentState = ScreenState::TEMP_HUMIDITY_SCREEN; break;
        case ScreenState::NEWS_SCREEN: currentState = ScreenState::WEATHER_SCREEN; break;
        default: break;
    }
}

void ButtonHandler::changeStateRight() {
    switch (currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: currentState = ScreenState::TEMP_HUMIDITY_SCREEN; break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: currentState = ScreenState::WEATHER_SCREEN; break;
        case ScreenState::WEATHER_SCREEN: currentState = ScreenState::NEWS_SCREEN; break;
        case ScreenState::NEWS_SCREEN: currentState = ScreenState::ALARM_SCREEN_VIEW; break;
        default: break;
    }
}

void ButtonHandler::changeSubState() {
    screenHandler.changeSubScreen();
    currentSubState = static_cast<SubScreenState>(screenHandler.getCurrentSubScreenNumber());
}

void ButtonHandler::changeTimeState() {
    currentTimeState = alarmScreen.changeTimeState();
}
