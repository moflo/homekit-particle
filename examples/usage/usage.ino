// Example usage for homekit library by moflo.

#include "homekit.h"

// Initialize objects from the lib
Homekit homekit;

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

    HomekitAccessory Lightbulb = homekit.newAccessory( lightbulbType, "light" );
    homekit.addAccessory( Lightbulb );
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

        uint8_t onOff = Lightbulb.state("on");
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
