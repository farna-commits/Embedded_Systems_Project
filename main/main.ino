//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"
#include <string.h>
#include <TinyProtocol.h>

//Setup 
void setup() {

  Serial.begin(9600);  
  while (!Serial) continue;
  Serial.setTimeout(0);
  //variables 
  char * ID_string;
  int ID_example = 0; 
  

  ID_string = (char*)calloc(16, sizeof(char));                        //allocate memory, and automatically freed 

  Read_json(doc,json);                                                //read json file 
  ID_example = doc["ID"][9];                                         //fetch ID from json database 
  Serial.print("Fetching an ID from database as an example: ");
  Serial.println(ID_example);  
  align_ID_string(ID_example, ID_string);                             //concatinate with 12 0s to align for aes function 
  uint8_t public_key[16] = {};
  uint8_t secret_key[16] = {};                                           
  GENERATE_KEY(public_key, secret_key);                               //call macro that generates key using Diffie Hellman 
  AES_encrypt(public_key,ID_string);                                  //encrypt using aes128 
  AES_decrypt(public_key, ID_string);                                 //decryption test 

  //Hashing 
  char * hashed_string; 
  hashed_string = (char*)calloc(256, sizeof(char));     
  ProcessInputMessage(ID_string, hashed_string);
  Serial.println("Hashed printing from main method: ");  
  Serial.println(hashed_string);  

  //Communication 
  char * packet_ID; 
  uint8_t packetsize_ID = 16; 
  char * packet_dh;
  uint8_t packetsize_dh = 32; 
  packet_ID = (char*)calloc(packetsize_ID, sizeof(char)); 
  packet_dh = (char*)calloc(packetsize_dh, sizeof(char)); 
  strcpy(packet_ID, "Hello");
  strcpy(packet_dh, "Public Key");
  Serial.println("Printing ID sent with tinyproto");  
  send_packet(packetsize_ID, packet_ID, ID_HEADER);
  Serial.println();
  Serial.println("Printing Diffie Public Key sent with tinyproto");  
  send_packet(packetsize_dh, packet_dh, DIFFIE_PUBLIC_KEY);
  Serial.println();


}

//Loop 
void loop() {
 
}