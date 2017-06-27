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

void Homekit::doit()
{
//    Serial.println("called doit");
    
    accessory.Characteristics[0].intValue = 1;
    
}
