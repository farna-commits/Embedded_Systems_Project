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
  char * Original;
  Original = (char*)calloc(16, sizeof(char));    
  Original = "Hell"; 
  // char Original[16] = "Helloooooooooo";

  // char * Decrypted;
  // Decrypted = (char*)calloc(16, sizeof(char)); 
  char Decrypted[16];
  // put your setup code here, to run once:
    uint8_t public_key1[16] = {};
    uint8_t secret_key1[16] = {};    
    uint8_t secret_key2[16] = {};  
    int ID_example = 0; 
  //align_ID_string(ID_example, Original); 
    // Generate the secret value "f" and the public value "k".
    Curve25519::dh1(public_key1, secret_key1);
    AES_encrypt(public_key1,Original); 
    // Serial.println(strlen(Original));
    AES_decrypt(public_key1, Original);  
    Serial.println(Original);
    // Serial.println("Public key 1 printing: ");
    // for (int i = 0; i < 16; i++) Serial.print(public_key1[i]);
    // Serial.println();
    // Serial.println("Secret key 1 printing: ");
    // for (int i = 0; i < 16; i++) Serial.print(secret_key1[i]);
    // Serial.println();
    // // Generate the shared secret in "k" using the previous secret value "f".
    // if (!Curve25519::dh2(public_key1, secret_key1)) {
    //     // The received "k" value was invalid - abort the session.
    // Serial.println("encryption failed");
    // }
    // // The "k" value can now be used to generate session keys for encryption.
    // Serial.println("Key generated successfully");

      
    


    // Curve25519::dh1(public_key1, secret_key2);
    // Serial.println("Public key 2 printing: ");
    // for (int i = 0; i < 16; i++) Serial.print(public_key1[i]);
    // Serial.println();
    // Serial.println("Secret key 2 printing: ");
    // for (int i = 0; i < 16; i++) Serial.print(secret_key2[i]);
    // Serial.println();

    // // Generate the shared secret in "k" using the previous secret value "f".
    // if (!Curve25519::dh2(public_key1, secret_key2)) {
    //     // The received "k" value was invalid - abort the session.
    // Serial.println("encryption failed");
    // }
    // // The "k" value can now be used to generate session keys for encryption.
    // Serial.println("Key generated successfully");

    // AES_decrypt(public_key1, Decrypted);  
    // Serial.println(Decrypted);
}

void loop() {
  // put your main code here, to run repeatedly:

}
