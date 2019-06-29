#include <Arduino.h>

#include "def.h"
#include "Sensor.h"
#include "Tools.h"
#include "AppI2C.h"

Sensor::Sensor() {}

Sensor::~Sensor() {}

char *Sensor::getSoilMoisture(int sensorId) {
    unsigned int moisture = analogRead(sensorId);
    char *result = Tools::intToChar(moisture);
    return result;
}
