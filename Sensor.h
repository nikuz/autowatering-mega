#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

class Sensor {
public:
    Sensor();

    ~Sensor();

    static char *getSoilMoisture(int sensorId);
};

#endif /* Sensor_h */
