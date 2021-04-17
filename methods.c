#include "methods.h"

void read_ID() {
  int i=0;
  while (i<ID_SIZE)
  {
    key = keypad.getKey();
    if (key!= NO_KEY){
      array_ID[i] = key;
      i++;
    }
  }
}


uint8_t * generate_key(uint8_t * f,uint8_t * k) {
    // Generate the secret value "f" and the public value "k".
    Curve25519::dh1(k, f);


    // Generate the shared secret in "k" using the previous secret value "f".
    if (!Curve25519::dh2(k, f)) {
        // The received "k" value was invalid - abort the session.
    Serial.println("encryption failed");
    }
    // The "k" value can now be used to generate session keys for encryption.
    Serial.println("Key generated successfully");

    return k;
    
}

void AES_encrypt(char * data) {
    const uint8_t KEY_SIZE = 16; 
    uint8_t f[KEY_SIZE];
    uint8_t k[KEY_SIZE];

    uint8_t key[KEY_SIZE];
    //key = generate_key(f,k);
    aes128_enc_single(key, data);
    Serial.print("encrypted:");
    Serial.print(data[0]);      
    Serial.print(data[1]);
    Serial.print(data[2]);
    Serial.println(data[3]);

}