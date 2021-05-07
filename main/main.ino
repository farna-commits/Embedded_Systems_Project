//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"

//Setup 
void setup() {
  Serial.begin(BAUD_RATE);                                              //Set the speed of the communication 
  Serial.setTimeout(MAX_TIMEOUT);                                       //Set max ms to wait for serial data (No timeout)
  proto_door.enableCheckSum();                                          //Tinyproto checksum error correction enabler
  proto_door.beginToSerial();                                           //Redirect all protocol communication to Serial0 UART
  Println("Enter ID: ");                                                //Initial Message to enter the ID from user
}

//Loop 
void loop() {    
  if (flag_response_done) {
    read_ID();                                                          //Keypad Function 
    Print("Entered ID is: ");                                           //Message 
    for (int i = 0; i < ID_SIZE; i++) Print((char)array_ID[i]);         //Printing ID 
    Println();                                                          //New Line
    DH1(public_key_door, secret_key_door);                              //DH1 Function to generate door's secret & public key      
    Print("Door Public Key: ");                                         //Message 
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door[i]);       //Printing the public key
    Println();                                                          //New Line
    Print("Door Secret Key: ");                                         //Message 
    for (int i = 0; i < KEY_SIZE; i++) Print(secret_key_door[i]);       //Printing secret key
    Println();                                                          //New Line       
    send_packet_door(KEY_SIZE, public_key_door, DIFFIE_PUBLIC_KEY);     //sending the Public Key
    flag_response_done = false;                                         //Flag to indicate finishing the process
  }
  proto_door.run();                                                     //Keeps door ready to send and recieve 
}