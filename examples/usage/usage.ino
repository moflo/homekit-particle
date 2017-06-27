// Example usage for homekit library by moflo.

#include "homekit.h"

// Initialize objects from the lib
Homekit homekit;

TCPClient client;
byte server[] = { 74, 125, 224, 72 }; // Google

int LED = D1;              // LED connected to D1


void setup() {
    // Call functions on initialized library objects that require hardware

    HomekitAccessory Lightbulb = homekit.newAccessory( lightbulbType, "light" );
    homekit.addAccessory( Lightbulb );
    homekit.begin();

    if (client.connect(server, 80))
    {
        Serial.println("connected");
        client.println("GET /search?q=unicorn HTTP/1.0");
        client.println("Host: www.google.com");
        client.println("Content-Length: 0");
        client.println();
    }
    else
    {
        Serial.println("connection failed");
    }

  pinMode(LED, OUTPUT);    // sets pin as output

}

void loop() {

    if (client.available())
    {
        // Use the library's initialized objects and functions
        homekit.process(  /* server */ );

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
