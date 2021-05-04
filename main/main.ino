//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"
//#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"
// #include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\methods_new.c"
// #include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\hashing.c"

#include <string.h>
#include <TinyProtocol.h>



//Setup 
void setup() {

  Serial.begin(9600);  
  while (!Serial) continue;
  Serial.setTimeout(0); 
  Println();                                       
  // Read_json(doc,json);
}



//Loop 
void loop() {
  
  
  if (flag_response_done) {
    read_ID();
    //Printing ID 
    for (int i = 0; i < ID_SIZE; i++) Print((char)array_ID[i]);
    //DH1 -- Generating door public and secret keys
    DH1(public_key_door, secret_key_door);                               //call macro that generates key using Diffie Hellman 
    
    //Printing the public key to be sent and the secret key
    Println("this is the door PK: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door[i]);
    Println();
    Println("this is the door SK: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(secret_key_door[i]);
    Println();
    //sending the Public Key
    send_packet_door(KEY_SIZE, public_key_door, DIFFIE_PUBLIC_KEY);
    // Read_json(doc,json);
    flag_response_done = false;     
    Println("Done, ready for next ID"); 
    
  }
  proto_door.run();
  // Println(flag_response_done);
}