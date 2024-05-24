/**
 * @file temphum.cpp
 * @author Aristidis Akritidis
 */

#include "tempHum.h"
#include <cstdio>

Temphum::Temphum() :  i2c(PB_11,PB_10),  sensor(&i2c) { 
    BufferedSerial pc(USBTX,USBRX);
    sensor.init(NULL);
    sensor.enable();
}

Temphum::~Temphum() { }

void Temphum::displayTempHumScreen(DFRobot_RGBLCD1602 &lcd) {

    sensor.get_humidity(&hum);
    sensor.get_temperature(&temp);

    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("temp: %.2f", temp);
    lcd.setCursor(0, 1);
    lcd.printf("hum: %.2f", hum);
    ThisThread::sleep_for(100ms);
}


