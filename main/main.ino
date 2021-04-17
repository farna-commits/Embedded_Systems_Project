//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"
#include <string.h>

//Setup 
void setup() {
  //variables 
  uint8_t public_key[16] = {};
  uint8_t secret_key[16] = {};
  char ID_string[16];
  int ID_example; 

  Serial.begin(9600);  
  while (!Serial) continue;
  Read_json(doc,json);                                                //read json file 
  ID_example = doc["ID"][49];                                         //fetch ID from json database 
  Serial.print("Fetching an ID from database as an example: ");
  Serial.println(ID_example);  
  itoa(ID_example, ID_string, 10);                                    //Convert int to string
  strcat(ID_string,"00000000000");                                    //concatinate with 12 0s to align for aes function   
  GENERATE_KEY(public_key, secret_key);                               //call macro that generates key using Diffie Hellman 
  AES_encrypt(public_key,ID_string);                                  //encrypt using aes128 
}

//Loop 
void loop() {
 
}