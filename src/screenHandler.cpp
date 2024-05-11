#include "screenHandler.h"

ScreenHandler::ScreenHandler(int maxScreenNumber, int currentScreenNumber, int currentSubScreenNumber, int maxSubScreenNumber) {
    this->maxScreenNumber = maxScreenNumber;
    this->currentScreenNumber = currentScreenNumber;
    this->currentSubScreenNumber = currentSubScreenNumber;
    this->maxSubScreenNumber = maxSubScreenNumber;
}

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

int ScreenHandler::getCurrentScreenNumber() {
    return currentScreenNumber;
}

int ScreenHandler::getMaxScreenNumber() {
    return maxScreenNumber;
}

int ScreenHandler::getCurrentSubScreenNumber() {
    return currentSubScreenNumber;
}