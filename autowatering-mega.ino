#include "Relay.h"
#include "AppSerial.h"
#include "AppI2C.h"
#include "AppTime.h"
#include "Sensor.h"
#include "Tools.h"
#include "def.h"

int TimeReadInterval = 1;  // read sensor every second
unsigned long TimeReadLastTime = millis();

#if PRODUCTION
int SensorsReadInterval = 60;  // read sensor once in minute
#else
int SensorsReadInterval = 2;  // read sensor once in 2 seconds
#endif
unsigned long SensorsReadLastTime = millis();

bool buttonIsPressed = false;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.print("Version: ");
    Serial.println(VERSION);

    Serial1.begin(115200);
    while (!Serial1) {
        ;
    }

    pinMode(BUTTON_PIN, INPUT);

    AppI2C::initiate();
    DEBUG_PRINTLN("AppI2C initiated");
#ifdef DEBUG
    AppI2C::scan();
#endif
    Relay::initiate();
    DEBUG_PRINTLN("Relay initiated");
    AppTime::RTCBegin();
    DEBUG_PRINTLN("AppTime initiated");

    DEBUG_PRINTLN("Setup done");

    delay(1000);
}

void loop() {
    SerialFrame newSerialFrame = AppSerial::getFrame();
    if (strcmp(newSerialFrame.command, "") != 0) {
        // relay
        bool performRelayCommand = Relay::parseSerialCommand(newSerialFrame.command, newSerialFrame.param);
        if (performRelayCommand) {
            AppSerial::sendFrame(&newSerialFrame);
        }
        // sync RTC time by NTP
        AppTime::RTCDateTimeUpdate(newSerialFrame.command, newSerialFrame.param);
//        // serial speed test
//        if (strcmp(newSerialFrame.command, "test") == 0) {
//            unsigned long testStart = millis();
//            SerialFrame testFirstFrame = SerialFrame("test", "first");
//            AppSerial::sendFrame(&testFirstFrame);
//            while (millis() - testStart < 1000) {
//                unsigned long val = millis() - testStart;
//                char *testStr = Tools::intToChar(val);
//                SerialFrame testFrame = SerialFrame("test", testStr);
//                AppSerial::sendFrame(&testFrame);
//            }
//            SerialFrame testLastFrame = SerialFrame("test", "last");
//            AppSerial::sendFrame(&testLastFrame);
//        }
    }

    // RTC time and temperature
    if (Tools::timerCheck(TimeReadInterval, TimeReadLastTime)) {
        const char *timeParam = AppTime::RTCGetCurrentTime();
        SerialFrame timeFrame = SerialFrame("time", timeParam);
        AppSerial::sendFrame(&timeFrame);

        const char *timeTempParam = AppTime::RTCGetTemperature();
        SerialFrame timeTemperatureFrame = SerialFrame("ttemp", timeTempParam);
        AppSerial::sendFrame(&timeTemperatureFrame);

        const char *timeBatteryParam = AppTime::RTCBattery();
        SerialFrame timeBatteryFrame = SerialFrame("tbtr", timeBatteryParam);
        AppSerial::sendFrame(&timeBatteryFrame);

        TimeReadLastTime = millis();
    }

    // sensors
    if (Tools::timerCheck(SensorsReadInterval, SensorsReadLastTime)) {
        int soilSensors[] = {
            SOIL_SENSOR_1,
            SOIL_SENSOR_2,
            SOIL_SENSOR_3,
            SOIL_SENSOR_4,
            SOIL_SENSOR_5,
            SOIL_SENSOR_6,
            SOIL_SENSOR_7,
            SOIL_SENSOR_8,
        };

        int soilSensorsCounts = *(&soilSensors + 1) - soilSensors;
        for (int i = 0; i < soilSensorsCounts; i++) {
            const char command[] = "sm";
            const char *commandEnd = Tools::intToChar(i + 1);
            strcat(command, commandEnd);
            const char *soilHumidityParam = Sensor::getSoilMoisture(soilSensors[i]);
            SerialFrame soilHumidityFrame = SerialFrame(command, soilHumidityParam);
            AppSerial::sendFrame(&soilHumidityFrame);
        }

        SerialFrame uptimeFrame = SerialFrame("uptime", Tools::getUptime());
        AppSerial::sendFrame(&uptimeFrame);

        SensorsReadLastTime = millis();
    }

    bool buttonPressedNow = digitalRead(BUTTON_PIN) == HIGH;
    if (buttonPressedNow && !buttonIsPressed) {
        buttonIsPressed = true;
    } else if (!buttonPressedNow && buttonIsPressed) {
        buttonIsPressed = false;
        Serial.println("button");
        SerialFrame buttonFrame = SerialFrame("btn", "1");
        AppSerial::sendFrame(&buttonFrame);
    }
}
