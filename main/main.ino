#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.h"
#include <string.h>




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
while (!Serial) continue;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }  

  int temp = doc["ID"][1];
  
  Serial.println(temp);
  char buf[16];
  // Convert number to string [buf]
  itoa(temp, buf, 10);
  Serial.println(buf);
  strcat(buf,"00000000000");
  Serial.println(buf);
  // uint8_t a[32], b[32], c[32]; 
  uint8_t key[16];
  uint8_t b[16];
  uint8_t *c; 
  //c = generate_key(a,b);
  Curve25519::dh1(key, b);
  Serial.println(key[0]);
  aes128_enc_single(key, buf);
  Serial.print("encrypted:");
  Serial.println(buf);
  aes128_dec_single(key, buf);
  Serial.print("decrypted:");
  Serial.print(buf[0]);
  Serial.print(buf[1]);
  Serial.print(buf[2]);
  Serial.print(buf[3]);
  // AES_encrypt("123");
}

void loop() {
//   int temp = doc["ID"][2];
//  Serial.println("test");
//   put your main code here, to run repeatedly:
//   read_ID();
//   Serial.println("test9");
//   Serial.println(array_ID[0]);
//  Serial.println(array_ID[1]);
//  Serial.println(array_ID[2]);
//   Serial.println(array_ID[3]);
 
}