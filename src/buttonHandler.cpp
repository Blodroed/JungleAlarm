#include "buttonHandler.h"
#include "mbed_events.h"

void ButtonHandler::handleLeftButton() {
    switch(currentSubState) {
        case SubScreenState::NO_STATE:
            changeStateLeft();
            break;
        case SubScreenState::SET_ALARM_SCREEN:
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
            alarmScreen.setAlarmTimeMore();
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
            break;
        case SubScreenState::SET_IP_ADDRESS:
            break;
        default:
            break;
    }

}

void ButtonHandler::handleMiddleButton() {
    if (currentSubState == SubScreenState::NO_STATE) {
        // DO NOT, I REPEAT, DO NOT RUN FUNCTIONS INSIDE THIS SWITCH CASE
        switch (currentState) {
            case ScreenState::ALARM_SCREEN_VIEW:
                eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
                if(alarmScreen.getAlarmActive()) {
                    alarmScreen.snoozeAlarm();
                    break;
                }
                if(!alarmScreen.getAlarmActive()) {
                    currentSubState = SubScreenState::SET_ALARM_SCREEN;
                }
                eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
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
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
            changeTimeState();
            if(alarmScreen.stateOfSettingAlarm == SettingAlarmState::ACCEPT) {
                alarmScreen.convertAlarmTimeToStruct();
                alarmScreen.enableAlarm();
                alarmScreen.stateOfSettingAlarm = SettingAlarmState::SET_ALARM_HOUR1;
                currentSubState = SubScreenState::NO_STATE;
            }
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
            break;
        case SubScreenState::SET_IP_ADDRESS: break;
        default: break;
    }
}

void ButtonHandler::handleRightButton() {
    switch (currentSubState) {
        case SubScreenState::NO_STATE:
            changeStateRight();
            break;
        case SubScreenState::SET_ALARM_SCREEN:
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
            alarmScreen.setAlarmTimeLess();
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
            break;
        case SubScreenState::SET_IP_ADDRESS:
            break;
        default:
            break;
    }

}

void ButtonHandler::handleSpecialButton() {
    if (currentSubState == SubScreenState::NO_STATE) {
        switch (currentState) {
            case ScreenState::ALARM_SCREEN_VIEW:
                eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
                if(alarmScreen.getAlarmActive() || alarmScreen.getAlarmSnoozed() > 0) {
                    alarmScreen.muteAlarm();
                }
                eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
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
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::lockMutex));
            alarmScreen.alarmSwitch();
            alarmScreen.convertAlarmTimeToStruct();

            alarmScreen.stateOfSettingAlarm = SettingAlarmState::SET_ALARM_HOUR1;
            currentSubState = SubScreenState::NO_STATE;
            eventQueue.call(callback(&alarmScreen, &AlarmScreen::unlockMutex));
            break;
        case SubScreenState::SET_IP_ADDRESS: break;
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

void ButtonHandler::changeToAlarmScreen() {
    if (currentState != ScreenState::ALARM_SCREEN_VIEW && currentSubState == SubScreenState::NO_STATE) {
        currentState = ScreenState::ALARM_SCREEN_VIEW;
        currentSubState = SubScreenState::NO_STATE;
    }
}
