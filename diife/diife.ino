#include <Key.h>
#include <Keypad.h>
#include <stdint.h>
#include <ArduinoJson.h>
#include <Curve25519.h>
#include <AESLib.h>
#include <TinyProtocol.h>
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.c"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  Serial.setTimeout(0);
  uint8_t public_key1[16] = {};
  uint8_t public_key2[16] = {};
  uint8_t secret_key1[16] = {};    
  uint8_t secret_key2[16] = {};  
  int ID_example = 0; 
  char * Original;
  Original = (char*)calloc(16, sizeof(char));    
  char * Decrypted;
  Decrypted = (char*)calloc(16, sizeof(char)); 
  char * Original_copy;
  Original_copy = (char*)calloc(16, sizeof(char)); 
  strcpy(Original, "Hello osama");
  strcpy(Original_copy, Original);
      
  //1st key 
  Curve25519::dh1(public_key1, secret_key1);
  Serial.println("Public key 1 printing: ");
  for (int i = 0; i < 16; i++) Serial.print(public_key1[i]);
  Serial.println();
  Serial.println("Secret key 1 printing: ");
  for (int i = 0; i < 16; i++) Serial.print(secret_key1[i]);
  Serial.println();

  //Rx
  Curve25519::dh1(public_key2, secret_key2);
  if (!Curve25519::dh2(public_key1, secret_key2)) Serial.println("encryption failed");
  Serial.println("Key generated successfully");


  AES_encrypt(public_key1,Original_copy); 
  AES_decrypt(public_key1, Original_copy);  
  Serial.println();
  strcpy(Decrypted, Original_copy);
  Serial.println("Decryption from main: ");
  Serial.println(Decrypted);


}

void loop() {
  // put your main code here, to run repeatedly:

}
