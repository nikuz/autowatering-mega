//#define PRODUCTION 1    // production
#define PRODUCTION 0    // development
//#define DEBUG 1

#define VERSION_ID "1"

#if PRODUCTION
#define VERSION_MARKER "P"
#else
#define VERSION_MARKER "D"
#endif

#define VERSION VERSION_MARKER VERSION_ID

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#define SOIL_SENSOR_1 12  // A12
#define SOIL_SENSOR_2 11  // A11
#define SOIL_SENSOR_3 10  // A10
#define SOIL_SENSOR_4 9   // A9
#define SOIL_SENSOR_5 8   // A8
#define SOIL_SENSOR_6 7   // A7
#define SOIL_SENSOR_7 6   // A6
#define SOIL_SENSOR_8 5   // A5

// relays defined in Relay.cpp
