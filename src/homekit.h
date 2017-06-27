#pragma once

/* homekit library by moflo
 */

// This will load the definition for common Particle variable types
#if defined(PARTICLE)
#   include "application.h"
#   include "Particle.h"
#else
struct TCPClient {
    int type;
};
#endif


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



// Services Type
typedef enum HomekitAccessoryType {
    infoType = 0x3E,
    lightbulbType = 0x43,
    switchType = 0x49,
    thermostatType = 0x4A,
    doorType = 0x81,
    smokeSensorType = 0x87,
    airQualitySensorType = 0x8D,
    batteryServiceType = 0x96,
    bridgeConfigurationType = 0xA1,
    bridgingStateType = 0x62,
    humiditySensorType = 0x82,
    temperatureSensorType = 0x8A,
    outletType = 0x47,
    windowType = 0x8B,
    windowCoveringType = 0x8C,
    fanType = 0x40,
    garageDoorOpenerType = 0x41,
    lockMechanismType = 0x45,
    securitySystemType = 0x7E

} HomekitAccessoryType_t;

// Charactersitics type
typedef enum CharacteristicType {
    onSetting = 0x25,
    brightnessSetting = 0x8,
    saturationSetting = 0x2F,
    hueSetting = 0x13,
    currentHumiditySetting = 0x10,
    currentTemperatureSetting = 0x11,
    targetTemperatureSetting = 0x35,
    currentHeatingCoolingStateSetting = 0xF,
    targetHeatingCoolingState =  0x33,
    temperatureDisplayUnitsSetting = 0x36,
    identifySetting = 0x14,
    manufacturerSetting = 0x20,
    modelSetting = 0x21,
    nameSetting = 0x23,
    serialNumberSetting = 0x30,
    currentPositionSetting = 0x6D,
    positionStateSetting = 0x72,
    targetPositionSetting = 0x7C,
    airQualitySetting = 0x95,
    batteryLevelSetting = 0x68,
    chargingStateSetting = 0x8F,
    statusLowBatterySetting = 0x79,
    configureBridgedAccessoryStatusSetting = 0x9D,
    discoverBridgedAccessoriesSetting = 0x9E,
    discoveredBridgedAccessoriesSetting = 0x9F,
    configureBridgedAccessorySetting = 0xA0,
    reachableSetting = 0x63,
    linkQualitySetting = 0x9C,
    accessoryIdentifierSetting = 0x57,
    categorySetting = 0xA3,
    outletInUseSetting = 0x26,
    currentDoorStateSetting = 0xE,
    targetDoorStateSetting = 0x32,
    obstructionDetectedSetting = 0x24,
    lockCurrentStateSetting = 0x1D,
    lockTargetStateSetting = 0x1E,
    securitySystemCurrentStateSetting = 0x66,
    securitySystemTargetStateSetting = 0x67

} CharacteristicType_t;

struct Characterist {
  CharacteristicType_t type;
  uint8_t size;
  uint16_t intValue;
  char * value;
  char * name;
};

// Accessory Structure

#define MAX_CHARACTERISTS 5

struct HomekitAccessory {
  HomekitAccessoryType_t type;
  uint8_t iid;
  char * name;
  struct Characterist Characteristics [ MAX_CHARACTERISTS ];
  uint8_t count;
};

// This is your main class that users will import into their application
class Homekit
{
    
public:
    Homekit();
    Homekit( HomekitAccessoryType type );
    Homekit( HomekitAccessoryType type, const char *name );
    ~Homekit();
    HomekitAccessoryType getType();
    const char * getName();
    int getCharacteristicCount();
    
    HomekitAccessory newAccessory( HomekitAccessoryType type );
    Characterist * newCharacteristic( CharacteristicType type );
    void begin();
    void process( TCPClient client );

private:
    HomekitAccessory accessory;
    char *name;
    
  void doit();
};
