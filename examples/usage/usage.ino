// Example usage for homekit library by moflo.

#include "homekit.h"

// Initialize objects from the lib
Homekit homekit;

void setup() {
    // Call functions on initialized library objects that require hardware
    homekit.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    homekit.process();
}
