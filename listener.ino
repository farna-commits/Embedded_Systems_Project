/*
 * This simple example listens for commands from UART.
 * This is demonstration of Half Duplex protocol.
 */
#include <TinyProtocol.h>

char received[5];

/* Function to receive incoming messages from remote side */
void onFrameIn(uint8_t *buf, int len)
{
    /* Do what you need with receive data here */
    for (int i=0; i<5; i++){
    received[i]=(char)buf[i];
    }
    //strcpy(received,(char)buf);
    Serial.println(received);
    Serial.println(strlen(received));
    Serial.println(sizeof(received));
}

/* Half Duplex protocol requires some buffer to store incoming data */
uint8_t buffer[5];

/* Creating Half Duplex protocol object is simple           */
/* Just pass buffer info and callback for incoming messages */
Tiny::ProtoHd  proto(buffer, sizeof(buffer), onFrameIn);



void setup() {
    /* No timeout, since we want non-blocking UART operations */
    Serial.setTimeout(0);
    /* Initialize serial protocol for test purposes */
    Serial.begin(115200);
    /* We do want to use simple checkSum */
    proto.enableCheckSum();
    /* Redirect all protocol communication to Serial0 UART */
    proto.beginToSerial();
   // for (int i=0; i<64; i++)
    //Serial.print(buffer[i]);
}


void loop()
{
    proto.run();
}