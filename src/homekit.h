#pragma once

/* homekit library by moflo
 */

//#ifndef homekit_h
//#define homekit_h 1

// This will load the definition for common Particle variable types
#if defined(PARTICLE)
#   include "application.h"
#   include "Particle.h"
#else
#   include "test.h"
#endif

#include "TLV8.h"

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
    char * manufacturer;
    char * model;
    char * serialNumber;
    struct Characterist Characteristics [ MAX_CHARACTERISTS ];
    uint8_t count;
    
};

typedef enum PairingCodesTLV8 {
    // Methods Table 4.4
    kTLVType_Method_None = 0x00,
    kTLVType_Method_PairSetup = 0x01,
    kTLVType_Method_PairVerify = 0x02,
    kTLVType_Method_AddPairing = 0x03,
    kTLVType_Method_RemovePairing = 0x04,
    kTLVType_Method_ListPairings = 0x05,
    
    // States
    kTLVType_State_None = 0x00,
    kTLVType_State_M1 = 0x01,
    kTLVType_State_M2 = 0x02,
    kTLVType_State_M3 = 0x03,
    kTLVType_State_M4 = 0x04,
    kTLVType_State_M5 = 0x05,
    kTLVType_State_M6 = 0x06,
    
    
    // Errors Table 4.5
    kTLVError_Unknown = 0x01,
    kTLVError_Authentication = 0x02,
    kTLVError_Backoff = 0x03,
    kTLVError_MaxPeers = 0x04,
    kTLVError_MaxTries = 0x05,
    kTLVError_Unavailable = 0x06,
    kTLVError_Busy = 0x07,
    
    // Data Types Table 5.6
    kTLVType_Method = 0x01,
    
    kTLVType_Identifier_Device = 0x01,
    kTLVType_Identifier_Accessory = 0x01,
    
    kTLVType_Salt = 0x02,
    
    kTLVType_PublicKey_Accessory = 0x03,
    kTLVType_PublicKey_Device = 0x03,
    
    kTLVType_Proof_Device = 0x04,
    kTLVType_Proof_Accessory = 0x04,
    
    kTLVType_EncryptedData_Device = 0x05,
    kTLVType_EncryptedData_Accessory = 0x05,
    
    kTLVType_State = 0x06,
    
    kTLVType_Error = 0x07,
    
    kTLVType_RetryDelay = 0x08,
    kTLVType_Certificate = 0x09,
    
    kTLVType_Signature_Device = 0x0A,
    kTLVType_Signature_Accessory = 0x0A,
    
    kTLVType_Permissions = 0x0B,
    kTLVType_FragmentData = 0x0C,
    kTLVType_FragmentLast = 0x0D,
    kTLVType_Separator = 0xFF
    
    
} PairingCodesTLV8_t;

struct HomeKitPairingState {

    PairingCodesTLV8_t state = kTLVType_Method_None;
    PairingCodesTLV8_t method = kTLVType_Method_None;
    
    long pairings[3];
    int pairing_count = 0;
    uint8_t * pk;
    uint8_t * sk;
    uint8_t * uuid;
    
    tlv_map_t commandTLV;
    
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
    
    HomeKitPairingState pairingState;
    
    void router(int method, const char *route);
    void doit();
    
    void respondControllerPairSetup();
    
};

//#endif /* homekit */
