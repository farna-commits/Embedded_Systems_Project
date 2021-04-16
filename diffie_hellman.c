#include <stdint.h>
#include "diffie_hellman.h"

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