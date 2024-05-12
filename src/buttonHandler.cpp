#include "buttonHandler.h"
#include <chrono>
#include <ratio>

void ButtonHandler::handleLeftButton() {
    switch(currentSubState) {
        case SubScreenState::NO_STATE: changeStateLeft(); break;
        case SubScreenState::SET_ALARM_SCREEN: break;
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
    if(currentSubState != SubScreenState::NO_STATE) {
        currentSubState = SubScreenState::NO_STATE;
        return;
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

