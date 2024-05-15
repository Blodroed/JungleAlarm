#include "mbed.h"
#include "../HTS221/HTS221Sensor.h"


DevI2C i2c(PB_11,PB_10);
HTS221Sensor tempHum(&i2c);

