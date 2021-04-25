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
//----------------------------Communication-----------------------------

Tiny::ProtoHd  proto_database (proto_buffer_database, sizeof(proto_buffer_database),  onFrameIn_database);
Tiny::ProtoHd  proto_door     (proto_buffer_door,     sizeof(proto_buffer_door),      onFrameIn_door);

/* Function to receive incoming messages from remote side */
void onFrameIn_database(char *buf, int len)
{

 
    Serial.print("the received packet size is :");
    Serial.println(len);
    Serial.print("the received packet is :");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    if(buf[0]== ID_HEADER && flag_ID_done == true) 
    {
      Serial.println("This is the ID packet"); //add code for what to do with data in case it is an ID 
      Tiny::Packet<64> packet;
      packet.clear();
      packet.put( "Received ID Packet" );          
      proto_database.write(packet);
      char * buf_copy;
      buf_copy = (char*)calloc(16, sizeof(char)); 
      for (int i = 1; i < len; i++)
      {
        strcat(buf_copy[i], buf[i]);
      }
      Serial.println(buf_copy);
      AES_decrypt(public_key, buf_copy);

    } 
    else if(buf[0] == DIFFIE_PUBLIC_KEY)
    {
      Serial.println("This is the key packet");//add code for what to do with data in case it is a public key
      Tiny::Packet<64> packet;
      packet.clear();
      packet.put( "Key Packet Received" );    
      proto_database.write(packet);

      DH1(public_key2, secret_key2); 
      char * buf_copy;
      buf_copy = (char*)calloc(16, sizeof(char)); 
      for (int i = 1; i < len; i++)
      {
        strcat(buf_copy[i], buf[i]);
      }
      Serial.println(buf_copy);
      DH2(buf_copy, secret_key2);
      flag_ID_done = true;


    }
    
}

void onFrameIn_door(char *buf, int len)
{
    /* Do what you need with receive data here */
    Serial.print("the received packet size is :");
    Serial.println(len);
    Serial.print("the received packet is :");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    if(buf[0]==ACK_KEY) 
    {
      Serial.println("The database received the Key packet"); //add code for what to do with data in case it is an ID 
      send_packet(32, "Public Key", DIFFIE_PUBLIC_KEY);
      flag_ID_ack_done = true;
      int ID_example = 0; 
      Read_json(doc,json);                                                //read json file 
      ID_example = doc["ID"][9];                                         //fetch ID from json database 
      Serial.print("Fetching an ID from database as an example: ");
      Serial.println(ID_example);  
      char * ID_string;
      ID_string = (char*)calloc(16, sizeof(char));  
      align_ID_string(ID_example, ID_string);  
      AES_encrypt(public_key,ID_string); 
      send_packet(16, ID_string, ID_HEADER);
    } 
    else if(buf[0]==ACK_ID && flag_ID_ack_done == true)
    {
        Serial.println("The database received the key packet");//add code for what to do with data in case it is a public key
    
    }
   
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