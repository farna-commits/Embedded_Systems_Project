#include "AES128.h"

void AES_encrypt(char * data) {
    const uint8_t KEY_SIZE = 16; 
    uint8_t f[KEY_SIZE];
    uint8_t k[KEY_SIZE];

    uint8_t key[KEY_SIZE] = generate_key(f,k);
    aes128_enc_single(key, data);
    Serial.print("encrypted:");
    Serial.print(data[0]);      
    Serial.print(data[1]);
    Serial.print(data[2]);
    Serial.println(data[3]);

}