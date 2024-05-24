/**
 * @file tempHum.h
 * @author Aristidis Akritidis
 */

#include "mbed.h"
#include "HTS221Sensor.h"
#include "DFRobot_RGBLCD1602.h"

class Temphum {
public:
    Temphum();
    ~Temphum();

    void displayTempHumScreen(DFRobot_RGBLCD1602 &lcd);

private:

    DevI2C i2c;
    HTS221Sensor sensor;

    float temp = 10.2; // C
    float hum = 10.1;  // 0.98


};

