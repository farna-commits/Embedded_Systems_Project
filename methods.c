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

Tiny::ProtoHd  proto_database(proto_buffer_database, sizeof(proto_buffer_database), onFrameIn_database);
Tiny::ProtoHd  proto_door(proto_buffer_door, sizeof(proto_buffer_door), onFrameIn_door);

/* Function to receive incoming messages from remote side */
void onFrameIn_database(char *buf, int len)
{

 
    Serial.print("the received packet size is :");
    Serial.println(len);
    Serial.print("the received packet is :");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    if(buf[0]=='h') 
    {
       Serial.println("This is the ID packet"); //add code for what to do with data in case it is an ID 
      Tiny::Packet<64> packet;
      packet.clear();
      packet.put( "Received ID Packet" );    
      proto_database.write(packet);
    } 
    else
    {
        Serial.println("This is the key packet");//add code for what to do with data in case it is a public key
    }
    
    //Serial.println(strlen(received_packet));
    //Serial.println(sizeof(received_packet));
}

void onFrameIn_door(char *buf, int len)
{
    /* Do what you need with receive data here */
    Serial.print("the received packet size is :");
    Serial.println(len);
    Serial.print("the received packet is :");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    // if(buf[0]=='R') 
    // {
    //    Serial.println("This is the ID packet"); //add code for what to do with data in case it is an ID 
    //   proto_database.write("Received ID Packet");
    // } 
    // else
    // {
    //     Serial.println("This is the key packet");//add code for what to do with data in case it is a public key
    // }
    //Serial.println(strlen(received_packet));
    //Serial.println(sizeof(received_packet));
}

// void receive_packet()
// {
//     proto.enableCheckSum(); 
//     proto.beginToSerial();
//     proto.run();

// }



void send_packet(uint16_t packetSize, char * packet_to_send, Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Tiny::Packet<16> packet; 
  proto_door.enableCheckSum(); 
  proto_door.beginToSerial();
  packet.clear(); 

  packet.put(packet_header_to_send);          //add the packet header as the first byte 
  packet.put(packet_to_send);         //add the data to the packet 
  proto_door.write(packet);
  
}