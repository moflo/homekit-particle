# homekit

A Particle library for Apple's HomeKit. Uses MDNS, TLV8 and private crypto libraries to present a Photon device as a HomeKit accessory.

## homekit background

Apple's HomeKit group has recently provided accesses to their proprietary HomeKit Accessory Protocol specification for non-commercial use. This library is an implementation of that HAP protocol for the Particle.io Photon board, allowing you to control your device directly from the Home iOS app.

## Usage

Connect Photon hardware, add the homekit library to your project and follow this simple example. Once the code is running on your Photon board, open the Home app on your iOS device and select the "Particle" accessory. In the example below, the Particle will appear as a "lightbulb" within the Home app, allow you to turn an LED on the board on & off.


```
#include "homekit.h"

// Initialize objects from the lib
Homekit homekit = Homekit( lightbulbType );

// Create a TCP/HTTP service for local discovery and HAP accessory handling
TCPServer server = TCPServer(80);

// Create a EthernetBonjour service for MDNS registration & discovery
EthernetBonjourClass Bonjour;

int LED = D1;              // LED connected to D1


void setup() {
    WiFi.connect();

    server.begin();

    // Initialize EthernetBonjour service with local UDP reference
    Bonjour.setUDP( &udp );

    // Begin MDNS registration servcie with the name of the HAP accessory (will appear in Apple Home)
    Bonjour.begin("particle");

    // Start MDNS registration with TCP based service, using special HAP TXT records encoded as c-string
    Bonjour.addServiceRecord("particle._hap",
                                    80,
                                    MDNSServiceTCP,
                                    "\x4sf=1\x14id=3C:33:1B:21:B3:00\x6pv=1.0\x04\c#=1\x04s#=1\x4\ff=0\x04sf=1\x0Bmd=particle");

    Characterist *light = homekit.newCharacteristic( onSetting );

    homekit.begin();


    pinMode(LED, OUTPUT);    // sets pin as output

}

void loop() {

    // Process MDNS UDP traffic once per loop
    Bonjour.run();

    // Check on TCP client status
    TCPClient client = server.available();

    if (client){
    {
        // Use the library's initialized objects and functions
        homekit.process(  client );

        uint8_t onOff = light->intValue;
        Serial.println("Lightbulb is ", onOff );

        if ( onOff ) {
              digitalWrite(LED, HIGH); // sets the LED on
        }
        else {
              digitalWrite(LED, LOW);  // sets the LED off
        }

    }

    if (!client.connected())
    {

        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        for(;;);
    
    }


}
```

See the [examples](examples) folder for more details.

## Documentation


## LICENSE
Copyright 2017 moflo

Licensed under the GNU LPL license.
