/*
 * This simple example listens for commands from UART.
 * This is demonstration of Half Duplex protocol.
 */
#include <TinyProtocol.h>

char received[32];

/* Function to receive incoming messages from remote side */
void onFrameIn(char *buf, int len)
{
    /* Do what you need with receive data here */
    for (int i=0; i<len; i++){
    received[i]=buf[i];   
    Serial.print(received[i]);
    }
    //strcpy(received,(char)buf);
    Serial.println();
    Serial.println(len);
    Serial.println(received);
    Serial.println(received[4]);
    Serial.println(strlen(received));
    Serial.println(sizeof(received));
}

/* Half Duplex protocol requires some buffer to store incoming data */
char buffer[128];

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
   // for (int i=0; i<64; i++)
    //Serial.print(buffer[i]);
}


void loop()
{
    proto.run();
}