#include "methods.h"

#define DH1(X, Y)    Curve25519::dh1(X, Y)
#define DH2(X, Y)    Curve25519::dh2(X, Y)

void read_ID() {
  int i = 0;
  while (i < ID_SIZE)
  {
    key2 = keypad.getKey();
    if (key2 != NO_KEY){
      array_ID[i] = key2;
      i++;
    }
  }
}

DeserializationError Read_json(StaticJsonDocument<700>, char*) {
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }  
  return error; 
}

void align_ID_string(int ID_example, char a[16]) {
  itoa(ID_example, a, 10);
  strcat(a,"00000000000");
}

void AES_encrypt(uint8_t * key, char * buf ) {
  aes128_enc_single(key, buf);
  Serial.print("Encrypted ID:");
  Serial.println(buf);
  //Serial.println(strlen(buf));

}

void AES_decrypt(uint8_t * key, char * buf) {
  aes128_dec_single(key, buf);
  Serial.print("Decrypted ID:");
  for (int i = 0; i < strlen(buf); i++) Serial.print(buf[i]); 
  
}

//Tiny objects 
Tiny::ProtoHd  proto_database (proto_buffer_database, sizeof(proto_buffer_database),  onFrameIn_database);
Tiny::ProtoHd  proto_door     (proto_buffer_door,     sizeof(proto_buffer_door),      onFrameIn_door);

void onFrameIn_database(char *buf, int len) {

    //Case key 
    if(buf[0] == DIFFIE_PUBLIC_KEY && flag_key_done == false) {

        //Printing key packet 
        Serial.print("The received key packet is: "); 
        for (int i=0; i<len; i++) Serial.print(buf[i]);
        
        //DH1 
        DH1(public_key_database, secret_key_database); 
        Serial.println("this is the database PK: ");
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_database[i]);
        Serial.println();
        Serial.println("this is the database SK: ");
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(secret_key_database[i]);
        Serial.println();

        //Extracting key from packet  
        for (int i = 0; i < KEY_SIZE; i++) public_key_door_copy[i] = buf[i+1];
        Serial.print("The received public key copy is: "); 
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_door_copy[i]); Serial.println();
        
        //Send packet 
        flag_key_done = true; 
        send_packet(KEY_SIZE, public_key_database, DIFFIE_PUBLIC_KEY);

    }
    //Case ID 
    else if(buf[0]== ID_HEADER && flag_key_done == true) {

        //Printing ID packet 
        Serial.print("The received Encrypted ID packet is: "); 
        for (int i=0; i<len; i++) Serial.print(buf[i]); Serial.println();

        //DH2
        DH2(public_key_door_copy, secret_key_database);
        Serial.println("ABG key is: ");
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_door_copy[i]); Serial.println();

        //Extracting ID packet 
        char * buf_copy;
        buf_copy = (char*)calloc(len, sizeof(char)); 
        for (int i = 0; i < len - 1; i++) buf_copy[i] = buf[i+1];
        Serial.print("The encrypted ID copy is: "); 
        for (int i = 0; i < len - 1; i++) Serial.print(buf_copy[i]); Serial.println();


        //Decryption
        AES_decrypt(public_key_door_copy, buf_copy);

        //Send done 
        Tiny::Packet<64> packet;
        packet.clear();
        packet.put( "Now checking: " );          
        proto_database.write(packet);

    }
}
