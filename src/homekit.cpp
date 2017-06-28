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

// Method to respond to route information
// Uses current state information contained in this->pairingState
// Controller methods are responsible for state update & HTTP resposne generation

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
        
        if (method == HTTP_METHOD_POST)
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
        switch (method) {
            case HTTP_METHOD_PUT:
                respondControllerCharacteristicsPUT();
                break;

            case HTTP_METHOD_GET:
                respondControllerCharacteristicsGET();
                break;
                
            default:
                break;
        }
        
    }

    if( strstr( route, "/pairings" ) )
    {
        // Respond with device pairings information
        
    }

}


// Router method to respond to <M1> Pair Setup requests
// Uses current state information in this->pairingState
// Pushes HTTP response directly on wire

void Homekit::respondControllerPairSetup()
{
    // Parse and respond to /pair-setup call

    tlv_t requestTLV = pairingState.commandTLV.object[0];
    tlv_map_t response = tlv_map();
    
    switch (pairingState.state) {
            
        case kTLVType_State_None:
            // Start pairing process - SRP Start Response
            if (pairingState.commandTLV.count == 1
                && requestTLV.type == kTLVType_Method
                && requestTLV.data[0] == kTLVType_Method_PairSetup)
            {

                uint8_t key[17] = "DEADBEEFDEADBEEF";
                uint8_t salt[17] = "DEADBEEFDEADBEEF";
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M2) );
                response.insert( tlv(kTLVType_PublicKey_Accessory, key, 16) );
                response.insert( tlv(kTLVType_Salt, salt, 16) );
             
                pairingState.state = kTLVType_State_M3;
            }
            else
            {
                // Unexpected error send kTLVError_Unavailable
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M2) );
                response.insert( tlv(kTLVType_Error, kTLVError_Unavailable) );
             
                pairingState.state = kTLVType_State_None;

            }
            break;
            
        case kTLVType_State_M3:
            // Receive device pairing information - SRP Verify Request
            if (pairingState.commandTLV.count == 3
                && requestTLV.type == kTLVType_Method
                && requestTLV.data[0] == kTLVType_Method_PairSetup)
            {
                tlv_t publickKeyTLV = pairingState.commandTLV.object[1];
                tlv_t proofTLV = pairingState.commandTLV.object[2];
                
                uint8_t * device_key = (uint8_t *)publickKeyTLV.data;
                uint8_t * device_proof = (uint8_t *)proofTLV.data;
                
                uint8_t accesssory_proof[17] = "DEADBEEFDEADBEEF";
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M4) );
                response.insert( tlv(kTLVType_Proof_Accessory, accesssory_proof, 16) );
                
                pairingState.state = kTLVType_State_M5;

            }
            else
            {
                // Unexpected error send kTLVError_Unavailable
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M4) );
                response.insert( tlv(kTLVType_Error, kTLVError_Authentication) );
                
                pairingState.state = kTLVType_State_None;

            }
            break;
            
        case kTLVType_State_M5:
            // Receive key exchange request - SRP Exchange Request
            if (pairingState.commandTLV.count == 2
                && requestTLV.type == kTLVType_Method
                && requestTLV.data[0] == kTLVType_Method_PairSetup)
            {
                tlv_t dataTLV = pairingState.commandTLV.object[1];
                
                uint8_t * encrypted_data = (uint8_t *)dataTLV.data;
                uint8_t * auth_tag = (uint8_t *)dataTLV.data+16;
                
                uint8_t pairingID[17] = "DEADBEEFDEADBEEF";
                uint8_t publicKey[17] = "DEADBEEFDEADBEEF";
                uint8_t signature[17] = "DEADBEEFDEADBEEF";
                uint8_t authTag[17] = "DEADBEEFDEADBEEF";
                uint8_t ChaCha20[17] = "DEADBEEFDEADBEEF";
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M6) );
                response.insert( tlv(kTLVType_EncryptedData_Accessory, ChaCha20, 16) );
             
                pairingState.state = kTLVType_State_None;

                // Save state
                pairingState.pk = publicKey;
                pairingState.sk = signature;
                pairingState.pairing_count = 1;
                pairingState.pairings[0] = 0xDEADBEEF;  // atoi( pairingID );
                
            }
            else
            {
                // Unexpected error send kTLVError_Unavailable
                
                response.insert( tlv(kTLVType_State, kTLVType_State_M6) );
                response.insert( tlv(kTLVType_Error, kTLVError_Authentication) );
                
                pairingState.state = kTLVType_State_None;

            }
            break;
            
        default:
            pairingState.state = kTLVType_State_None;
            response.count = 0;
            break;
    }
    
    // Send serialized response
    
    
}


// Router method to respond to update (PUT) request
// Uses current state information in this->pairingState
// Updates characteristic state information based on JSON data
// Pushes HTTP response directly on wire

void Homekit::respondControllerCharacteristicsPUT()
{
    // Parse and respond to /pair-setup call
    
    tlv_t requestTLV = pairingState.commandTLV.object[0];
    tlv_map_t response;

    // Receive JSON PUT body
    /*
     {
        "characteristics" : [
         {
             "aid" : 2,
             "iid" : 8,
             "value" : true
         } 
        ]
     }
    */
    
    // Send response HTTP/1.1 204 No Content
}


// Router method to respond to polling (GET) request
// Uses current state information in this->pairingState
// Sends characteristic state information based on JSON data
// Pushes HTTP response directly on wire

void Homekit::respondControllerCharacteristicsGET()
{
    // Receive PUT parameters
    
    // GET /characteristics?id=2.8 HTTP/1.1
    // GET /characteristics?id=2.8,3.8 HTTP/1.1
    
    // Send response
    
    /*
     HTTP/1.1 200 OK
     Content-Type: application/hap+json
     Content-Length: <length>
     {
         "characteristics" : [
             {
             "aid" : 2,
             "iid" : 8,
             "value" : false,
             }
         ]
     }
    */
}

void Homekit::doit()
{
//    Serial.println("called doit");
    
    accessory.Characteristics[0].intValue = 1;
    
}
