//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"
#include <string.h>

#define msg_define "Hi my name is osama\0"
//Setup 
void setup() {

  Serial.begin(9600);  
  while (!Serial) continue;
  //variables 
  char * ID_string;
  int ID_example = 0; 
<<<<<<< HEAD
 // add_user();
=======
  

  ID_string = (char*)calloc(16, sizeof(char));                        //allocate memory, and automatically freed 

>>>>>>> 0a200795fc2633ab94f0a684b1c213d72ed567a9
  Read_json(doc,json);                                                //read json file 
  ID_example = doc["ID"][2];                                         //fetch ID from json database 
  Serial.print("Fetching an ID from database as an example: ");
  Serial.println(ID_example);  
  align_ID_string(ID_example, ID_string);                             //concatinate with 12 0s to align for aes function 
  uint8_t public_key[16] = {};
  uint8_t secret_key[16] = {};                                           
  GENERATE_KEY(public_key, secret_key);                               //call macro that generates key using Diffie Hellman 
  AES_encrypt(public_key,ID_string);                                  //encrypt using aes128 
  AES_decrypt(public_key, ID_string);                                 //decryption test 

  //Hashing 
  ProcessInputMessage(ID_string);


}

//Loop 
void loop() {
 
}