//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"

//Setup 
void setup() {     
    Serial.begin(BAUD_RATE);               //Set the speed of the communication 
    Serial.setTimeout(MAX_TIMEOUT);        //Set max ms to wait for serial data (No timeout)
    proto_database.enableCheckSum();       //Tinyproto checksum error correction enabler
    proto_database.beginToSerial();        //Redirect all protocol communication to Serial0 UART
}

//Loop 
void loop()
{
    proto_database.run();                   //Keeps database ready to send and recieve 
}
