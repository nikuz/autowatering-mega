#include <Arduino.h>

#include "def.h"
#include "Relay.h"

// RelayItem({pin}, {name}, {highLevelTrigger}) - highLevelTrigger is true by default
static RelayItem relays[] = {
    RelayItem(A1, "s1", false),
    RelayItem(A2, "s2", false),
    RelayItem(48, "s3", false),
    RelayItem(49, "s4", false),
    RelayItem(50, "s5", false),
    RelayItem(51, "s6", false),
    RelayItem(52, "s7", false),
    RelayItem(53, "s8", false),
};

const char relayOnSerialCommand[] = "rOn";
const char relayOffSerialCommand[] = "rOf";

Relay::Relay() {}

Relay::~Relay() {}

// public

void Relay::initiate() {
    int relaysCounts = *(&relays + 1) - relays;
    for (int i = 0; i < relaysCounts; i++) {
        pinMode(relays[i].pin, OUTPUT);
        // turn off relays by default
        // not all relays is HIGH level triggered
        if (relays[i].highLevelTrigger) {
            Serial.print("HIGH level trigger: ");
            Serial.println(relays[i].name);
            digitalWrite(relays[i].pin, LOW);
        } else {
            Serial.print("LOW level trigger: ");
            Serial.println(relays[i].name);
            digitalWrite(relays[i].pin, HIGH);
        }
    }
}

bool Relay::parseSerialCommand(const char *command, const char *param) {
    if (strcmp(command, relayOnSerialCommand) == 0)
        return Relay::on(param);
    if (strcmp(command, relayOffSerialCommand) == 0)
        return Relay::off(param);

    return false;
}

// private

bool Relay::on(const char *name) {
    RelayItem relayItem = Relay::getRelayPin(name);
    if (relayItem.pin != -1) {
        DEBUG_PRINT("Relay ON: ");
        DEBUG_PRINTLN(relayItem.name);
        if (relayItem.highLevelTrigger) {
            digitalWrite(relayItem.pin, HIGH);
        } else {
            digitalWrite(relayItem.pin, LOW);
        }
        return true;
    }

    DEBUG_PRINT("Relay doesn't exist: ");
    DEBUG_PRINTLN(name);
    return false;
}

bool Relay::off(const char *name) {
    RelayItem relayItem = Relay::getRelayPin(name);
    if (relayItem.pin != -1) {
        DEBUG_PRINT("Relay OFF: ");
        DEBUG_PRINTLN(relayItem.name);
        if (relayItem.highLevelTrigger) {
            digitalWrite(relayItem.pin, LOW);
        } else {
            digitalWrite(relayItem.pin, HIGH);
        }

        return true;
    }

    DEBUG_PRINT("Relay doesn't exist: ");
    DEBUG_PRINTLN(name);
    return false;
}

RelayItem Relay::getRelayPin(const char *name) {
    int relaysCounts = *(&relays + 1) - relays;
    for (int i = 0; i < relaysCounts; i++) {
        if (strcmp(relays[i].name, name) == 0) {
            return relays[i];
        }
    }

    return RelayItem(-1, "");
}
