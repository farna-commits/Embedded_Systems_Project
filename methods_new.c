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
        Serial.print("The received key packet from the door is: "); 
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
        send_packet_database(KEY_SIZE, public_key_database, DIFFIE_PUBLIC_KEY);

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

void onFrameIn_door(char *buf, int len) {

    //Case key
    if(buf[0] == DIFFIE_PUBLIC_KEY)
    {
        //Printing Key packet 
        Serial.print("The received key packet from the database is: "); 
        for (int i=0; i<len; i++) Serial.print(buf[i]); Serial.println();

        //Extracting key from packet  
        for (int i = 0; i < KEY_SIZE; i++) public_key_database_copy[i] = buf[i+1];
        Serial.print("The received public key copy is: "); 
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_database_copy[i]); Serial.println();


        //Fetch from example
        int ID_example = 0; 
        //Read_json(doc,json);                                                //read json file 
        ID_example = doc["ID"][9];                                         //fetch ID from json database 
        Serial.print("Fetching an ID from database as an example: ");
        Serial.println(ID_example);  
        char * ID_string;
        ID_string = (char*)calloc(64, sizeof(char));  
        // char ID_string[64]; 
        align_ID_string(ID_example, ID_string); 
        //After Alignment
        Serial.println("Aligned: ");
        Serial.println(ID_string);

        //DH2
        DH2(public_key_database_copy, secret_key_door);
        Serial.println("ABG key is: ");
        for (int i = 0; i < KEY_SIZE; i++) Serial.print(public_key_door_copy[i]); Serial.println();

        //Encryption
        AES_encrypt(public_key_database_copy, ID_string);


        //Print the encrypted data
        Serial.println("The encrypted ID is: ");
        for (int i = 0; i < len; i++) Serial.print(ID_string[i]); Serial.println();

        // Send the encrypted data
        flag_ID_ack_done = true;
        send_packet_door(KEY_SIZE, ID_string, ID_HEADER);
    }

    else if (buf[0] == ACK_ID && flag_ID_ack_done == true) {
        //Decision to open door or not
        Serial.println("The database bey2ool tamam wala la");
    }
}

void send_packet_door(uint16_t packetSize, char * packet_to_send, Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<64> packet; 
  proto_door.enableCheckSum(); 
  proto_door.beginToSerial();
  packet.clear(); 

  packet.put(packet_header_to_send);          //add the packet header as the first byte 
  packet.put(packet_to_send);         //add the data to the packet 
  proto_door.write(packet);
  
}


void send_packet_database(uint16_t packetSize, char * packet_to_send, Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<64> packet; 
  proto_database.enableCheckSum(); 
  proto_database.beginToSerial();
  packet.clear(); 

  packet.put(packet_header_to_send);          //add the packet header as the first byte 
  packet.put(packet_to_send);         //add the data to the packet 
  proto_database.write(packet);
  
}


void send_packet_door(uint16_t packetSize, uint8_t packet_to_send[KEY_SIZE], Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<64> packet; 
  proto_door.enableCheckSum(); 
  proto_door.beginToSerial();
  packet.clear(); 

  packet.put(packet_header_to_send);   //add the packet header as the first byte
  for (int i = 0; i < packetSize; i++) packet.put(packet_to_send[i]);
  
  proto_door.write(packet);
}

void send_packet_database(uint16_t packetSize, uint8_t packet_to_send[KEY_SIZE], Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<64> packet; 
  proto_database.enableCheckSum(); 
  proto_database.beginToSerial();
  packet.clear(); 

  packet.put(packet_header_to_send);   //add the packet header as the first byte
  for (int i = 0; i < packetSize; i++) packet.put(packet_to_send[i]);
  
  proto_database.write(packet);
}