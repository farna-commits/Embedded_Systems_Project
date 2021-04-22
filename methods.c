#include "methods.h"

#define GENERATE_KEY(X, Y)    Curve25519::dh1(X, Y)

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
  Serial.print(buf[0]);
  Serial.print(buf[1]);
  Serial.print(buf[2]);
  Serial.print(buf[3]);
}
// void add_user() {
//   read_ID();
//   for(int i=0 i< ID_SIZE; i++)
//     json[strlen(json)-(3-i)]=array_ID[i];
//   strcat(json, array_ID);

// }

//----------------------------Communication-----------------------------

/* Function to receive incoming messages from remote side */
void onFrameIn(uint8_t *buf, int len)
{
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) received_packet[i]=(char)buf[i];
    if(received_packet[0]=='h') 
    {
        //add code for what to do with data in case it is an ID 
    } 
    else
    {
        //add code for what to do with data in case it is a public key
    }
    Serial.print("the received packet is :");
    Serial.println(received_packet);
    Serial.print("the received packet size is :");
    Serial.println(len);
    //Serial.println(strlen(received_packet));
    //Serial.println(sizeof(received_packet));
}

Tiny::ProtoHd  proto(proto_buffer, sizeof(proto_buffer), onFrameIn);

void receive_packet()
{
    proto.enableCheckSum(); 
    proto.beginToSerial();
    proto.run();

}



void send_packet(uint16_t packetSize, char * packet_to_send, Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<16> packet; 
  proto.enableCheckSum(); 
  proto.beginToSerial();
  packet.clear(); 
  packet.put(packet_header_to_send);          //add the packet header as the first byte 
  packet.put(packet_to_send);         //add the data to the packet 
  proto.write(packet);
  
}