/*
 * This example sends <Hello> packet every second.
 * This is demontraction of Half Duplex protocol.
 */
#include <TinyProtocol.h>

/* Function to receive incoming messages from remote side */
void onFrameIn(char *buf, int len)
{
    /* Do what you need with receive data here */
}

/* Half Duplex protocol requires some buffer to store incoming data */
char buffer[64];

/* Creating Half Duplex protocol object is simple           */
/* Just pass buffer info and callback for incoming messages */
Tiny::ProtoHd  proto(buffer, sizeof(buffer), onFrameIn);

void setup() {
    /* No timeout, since we want non-blocking UART operations */
    Serial.setTimeout(0);
    /* Initialize serial protocol for test purposes */
    Serial.begin(9600);
    /* We do want to use simple checkSum */
    proto.enableCheckSum();
    /* Redirect all protocol communication to Serial0 UART */
    proto.beginToSerial();

/* Prepare data you want to send here */
    Tiny::Packet<64> packet;
    packet.clear();
    packet.put( "HELLOHELLO" );
    packet.put( "HOME" );
    packet.put( "HOMI" );

    /* Send packet over UART to other side */
    proto.write(packet);


}

void loop()
{
    
    /* We send HELLO packet every 1 second */
//    unsigned long start = millis();
//    while (millis() - start < 1000)
//    {
//        proto.run();
//    }
}