#include "screenHandler.h"
#include "../libs/DFRobot_RGBLCD1602/DFRobot_RGBLCD1602.h"

ScreenHandler::ScreenHandler(int currentScreenNumber, int maxScreenNumber, int currentSubScreenNumber, int maxSubScreenNumber) {
    this->maxScreenNumber = maxScreenNumber;
    this->currentScreenNumber = currentScreenNumber;
    this->currentSubScreenNumber = currentSubScreenNumber;
    this->maxSubScreenNumber = maxSubScreenNumber;
}

// changing screen functions
void ScreenHandler::changeScreenRight() {
    currentScreenNumber++;
    if(currentScreenNumber > maxScreenNumber) {
        currentScreenNumber = 0;
    }
    
}

void ScreenHandler::changeScreenLeft() {
    currentScreenNumber--;
    if(currentScreenNumber <= 0) {
        currentScreenNumber = maxScreenNumber;
    }
}

void ScreenHandler::changeSubScreen() {
    if(currentSubScreenNumber > 0) {
        currentSubScreenNumber = 0;
    } else {
        switch (currentScreenNumber) {
            case 0: currentSubScreenNumber = 1; break;
            case 1: currentScreenNumber = 2; break;
            default: break;
        }
    }
}

// Display alarm
void ScreenHandler::displayAlarm(DFRobot_RGBLCD1602 &lcd) {
    lcd.display();
    lcd.setCursor(0, 1);
    lcd.printf("Alarm");
    ThisThread::sleep_for(100ms);
    lcd.clear();
}

// Getters
int ScreenHandler::getCurrentScreenNumber() {
    return currentScreenNumber;
}

int ScreenHandler::getMaxScreenNumber() {
    return maxScreenNumber;
}

int ScreenHandler::getCurrentSubScreenNumber() {
    return currentSubScreenNumber;
}