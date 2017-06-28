/* homekit library by moflo
 */

#include "homekit.h"

Homekit::Homekit( )
{
//    Homekit(lightbulbType, "Particle");
    
    this->accessory = newAccessory( lightbulbType );
    
    const char hap_name[9] = "Particle";
    
    this->name = (char *)malloc( strlen(hap_name));
    
    memcpy(this->name, hap_name, strlen(hap_name));

}

Homekit::Homekit( HomekitAccessoryType type )
{
//    Homekit(type, "Particle");
 
    this->accessory = newAccessory( type );
    
    const char hap_name[9] = "Particle";
    
    this->name = (char *)malloc( strlen(hap_name));
    
    memcpy(this->name, hap_name, strlen(hap_name));
  
}

Homekit::Homekit( HomekitAccessoryType type, const char *hap_name )
{
    this->accessory = newAccessory( type );
    
    this->name = (char *)malloc( strlen(hap_name));
    
    memcpy(this->name, hap_name, strlen(hap_name));

}

Homekit::~Homekit()
{
    
}

HomekitAccessoryType Homekit::getType()
{
    return accessory.type;
}
const char * Homekit::getName()
{
    return name;
}
int Homekit::getCharacteristicCount()
{
    return accessory.count;
}


HomekitAccessory Homekit::newAccessory( HomekitAccessoryType type )
{
    HomekitAccessory object = HomekitAccessory();
    
    object.type = type;
    
    return object;
}

Characterist * Homekit::newCharacteristic( CharacteristicType type )
{
    Characterist object = Characterist();
    object.type = type;
    
    static char charName[5] = "Test";
    object.name = (char *)malloc( strlen(charName));
    
    memcpy(object.name, charName, strlen(charName));

    // Add to default accessory
    if (accessory.count < MAX_CHARACTERISTS) {
        accessory.Characteristics[ accessory.count ] = object;
        accessory.count += 1;
    }
    
    return &accessory.Characteristics[ accessory.count - 1 ];
}

// initialize hardware
void Homekit::begin()
{
//    Serial.println("called begin");
}


// Process loop method
void Homekit::process( TCPClient client )
{
    // do something useful
//    Serial.println("called process");
    doit();
}

void Homekit::router(int method, const char *route)
{

    if( strstr( route, "/" ) )
    {
        // Respond OK, prompt user to pair with iOS device
        
    }
    
    if( strstr( route, "/identify" ) )
    {
        // Respond with device identification
        
    }
    
    if( strstr( route, "/pair-setup" ) )
    {
        // Respond with pair setup state & challenge
        
        // method == POST
        respondControllerPairSetup();
        
    }
    
    if( strstr( route, "/pair-verify" ) )
    {
        // Respond with pair verification
        
    }
    
    if( strstr( route, "/accessories" ) )
    {
        // Respond with accessories information
        
    }
    
    if( strstr( route, "/characteristics" ) )
    {
        // Respond with characteristics information
        
    }

    if( strstr( route, "/pairings" ) )
    {
        // Respond with device pairings information
        
    }

}


void Homekit::respondControllerPairSetup()
{
    // Parse and respond to /pair-setup call

    tlv_t currentTLV = pairingState.commandTLV.object[0];
    tlv_map_t response;
    
    switch (pairingState.state) {
            
        case kTLVType_State_None:
            // Start pairing process
            if (pairingState.commandTLV.count == 1
                && currentTLV.type == kTLVType_Method
                && currentTLV.data[0] == kTLVType_Method_PairSetup)
            {

                uint8_t key[17] = "DEADBEEFDEADBEEF";
                uint8_t salt[17] = "DEADBEEFDEADBEEF";
                
                response.count = 3;
                response.object[0] = tlv(kTLVType_State, kTLVType_State_M2);
                response.object[1] = tlv(kTLVType_PublicKey_Accessory, key, 16);
                response.object[2] = tlv(kTLVType_Salt, salt, 16);
                
            }
            else
            {
                // Unexpected error send kTLVError_Unavailable
                
                response.count = 2;
                response.object[0] = tlv(kTLVType_State, kTLVType_State_M2);
                response.object[1] = tlv(kTLVType_Error, kTLVError_Unavailable);
                
            }
            break;
            
        default:
            break;
    }
    
    // Send serialized response
    
    
}

void Homekit::doit()
{
//    Serial.println("called doit");
    
    accessory.Characteristics[0].intValue = 1;
    
}
