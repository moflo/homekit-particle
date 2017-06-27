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
    onChar = 0x25,
    brightnessChar = 0x8,
    saturationChar = 0x2F,
    hueChar = 0x13,
    currentHumidityChar = 0x10,
    currentTemperatureChar = 0x11,
    targetTemperatureChar = 0x35,
    currentHeatingCoolingStateChar = 0xF,
    targetHeatingCoolingState =  0x33,
    temperatureDisplayUnitsChar = 0x36,
    identifyChar = 0x14,
    manufacturerChar = 0x20,
    modelChar = 0x21,
    nameChar = 0x23,
    serialNumberChar = 0x30,
    currentPositionChar = 0x6D,
    positionStateChar = 0x72,
    targetPositionChar = 0x7C,
    airQualityChar = 0x95,
    batteryLevelChar = 0x68,
    chargingStateChar = 0x8F,
    statusLowBatteryChar = 0x79,
    configureBridgedAccessoryStatusChar = 0x9D,
    discoverBridgedAccessoriesChar = 0x9E,
    discoveredBridgedAccessoriesChar = 0x9F,
    configureBridgedAccessoryChar = 0xA0,
    reachableChar = 0x63,
    linkQualityChar = 0x9C,
    accessoryIdentifierChar = 0x57,
    categoryChar = 0xA3,
    outletInUseChar = 0x26,
    currentDoorStateChar = 0xE,
    targetDoorStateChar = 0x32,
    obstructionDetectedChar = 0x24,
    lockCurrentStateChar = 0x1D,
    lockTargetStateChar = 0x1E,
    securitySystemCurrentStateChar = 0x66,
    securitySystemTargetStateChar = 0x67

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
  uint8_t type;
  uint8_t iid;
  char * name;
  struct Characterist Characteristics [ MAX_CHARACTERISTS ];
};

// This is your main class that users will import into their application
class Homekit
{
public:
  /**
   * Constructor
   */
  Homekit();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process( TCPClient client );

private:
  /**
   * Example private method
   */
  void doit();
};
