/*
 * This simple example listens for commands from UART.
 * This is demonstration of Half Duplex protocol.
 */
#include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\methods.c"
#include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\hashing.c"

#include <string.h>
#include <TinyProtocol.h>


void setup() {
    /* No timeout, since we want non-blocking UART operations */
    Serial.setTimeout(0);
    /* Initialize serial protocol for test purposes */
    Serial.begin(9600);
    /* We do want to use simple checkSum */
    proto_database.enableCheckSum();
    /* Redirect all protocol communication to Serial0 UART */
    proto_database.beginToSerial();
   

}


void loop()
{
    proto_database.run();
}