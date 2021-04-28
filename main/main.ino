//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods_new.c"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"
// #include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\methods_new.c"
// #include "C:\Users\Mahmoud Shamaa\Desktop\Embedded Systems\Project\Embedded_Systems_Project\hashing.c"

#include <string.h>
#include <TinyProtocol.h>

//Setup 
void setup() {

  Serial.begin(9600);  
  while (!Serial) continue;
  Serial.setTimeout(0); 
  Serial.println();                                       

  //DH1 -- Generating door public and secret keys
  DH1(public_key_door, secret_key_door);                               //call macro that generates key using Diffie Hellman 
  
  //Printing the public key to be sent and the secret key
  Serial.println("this is the door PK: ");
  for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_door[i]);
  Serial.println();
  Serial.println("this is the door SK: ");
  for (int i = 0; i < KEY_SIZE; i++) Serial.print(secret_key_door[i]);
  Serial.println();
  //sending the Public Key
  send_packet_door(KEY_SIZE, public_key_door, DIFFIE_PUBLIC_KEY);
  Read_json(doc,json);

  // //Hashing 
  // char * hashed_string; 
  // hashed_string = (char*)calloc(256, sizeof(char));     
  // ProcessInputMessage(ID_string, hashed_string);
  // Serial.println("Hashed printing from main method: ");  
  // Serial.println(hashed_string);  
}

//Loop 
void loop() {
 proto_door.run();
}