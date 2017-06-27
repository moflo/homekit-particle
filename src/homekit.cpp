/* homekit library by moflo
 */

#include "homekit.h"

/**
 * Constructor.
 */
Homekit::Homekit()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void Homekit::begin()
{
    // initialize hardware
//    Serial.println("called begin");
}

/**
 * Example method.
 */
void Homekit::process( TCPClient client )
{
    // do something useful
//    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void Homekit::doit()
{
//    Serial.println("called doit");
}
