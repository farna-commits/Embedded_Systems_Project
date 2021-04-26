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
    Serial.println();
    Serial.println("Printing flags: ");
    Serial.print("flag ID: ");
    Serial.println((flag_ID_done == true) ? "True" : "False");
    Serial.print("flag keep sending ID: ");
    Serial.println((flag_keep_sending_ID == true) ? "True" : "False");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    if(buf[0]== ID_HEADER && flag_ID_done == true) 
    {
      Serial.println("Here in ID if");
      Serial.println("This is the ID packet"); //add code for what to do with data in case it is an ID 
      
      char * buf_copy;
      buf_copy = (char*)calloc(64, sizeof(char)); 
      Serial.println("Copying buf: ");
      for (int i = 0; i < len-1; i++) buf_copy[i] = buf[i+1];
      for (int i = 0; i < len-1; i++) Serial.print(buf_copy[i]);
      Serial.println("Decryption: ");
      // uint8_t public_key_uint2[32]; 
      // memcpy(public_key_uint2, public_key, strlen(public_key)+1);
      AES_decrypt(buf_copy_key, buf_copy);

      Tiny::Packet<64> packet;
      packet.clear();
      packet.put( "Received ID Packet" );          
      proto_database.write(packet);

    } 
    else if(buf[0] == DIFFIE_PUBLIC_KEY && flag_ID_done == false)
    {
      Serial.println("This is the key packet");//add code for what to do with data in case it is a public key
      Serial.println();
      DH1(public_key2, secret_key2); 
      Serial.println("here");
      // char * buf_copy;
      // buf_copy = (char*)calloc(64, sizeof(char)); 
      // Serial.println("Buf Now: ");
      // for (int i = 0; i < len; i++) Serial.print(buf[i]);
      // Serial.println();
      Serial.println("Copying buf: ");
      for (int i = 0; i < len - 1; i++) buf_copy_key[i] = buf[i+1];
      Serial.println("Copy loop done");
      //Serial.println(buf_copy);
      // Serial.println("Buf Now: ");
      // for (int i = 0; i < len; i++) Serial.print(buf[i]);
      // Serial.println("Buf copy Now: ");
      for (int i = 0; i < len - 1; i++) Serial.print(buf_copy_key[i]);
      Serial.println();
      DH2(buf_copy_key, secret_key2);
      Serial.println("DH2 Done");
      flag_ID_done = true;
      Serial.println((flag_ID_done == true) ? "True" : "False");
      Tiny::Packet<64> packet;
      packet.clear();
      packet.put( "Key Packet Received" );    
      proto_database.write(packet);


    }
    
}

void onFrameIn_door(char *buf, int len)
{
    /* Do what you need with receive data here */
    Serial.print("the received packet size is :");
    Serial.println(len);
    Serial.print("the received packet is :");
    Serial.println();
    Serial.println("Printing flags: ");
    Serial.print("flag ID: ");
    Serial.println((flag_ID_done == true) ? "True" : "False");
    Serial.print("flag keep sending ID: ");
    Serial.println((flag_keep_sending_ID == true) ? "True" : "False");
    /* Do what you need with receive data here */
     for (int i=0; i<len; i++) Serial.print(buf[i]);
    if(buf[0]==ACK_KEY) 
    {
      Serial.println("The database received the Key packet"); //add code for what to do with data in case it is an ID 
      //send_packet(32, "Public Key", DIFFIE_PUBLIC_KEY);
      flag_ID_ack_done = true;
      int ID_example = 0; 
      //Read_json(doc,json);                                                //read json file 
      ID_example = doc["ID"][9];                                         //fetch ID from json database 
      Serial.print("Fetching an ID from database as an example: ");
      Serial.println(ID_example);  
      char * ID_string;
      ID_string = (char*)calloc(64, sizeof(char));  
      // char ID_string[32]; 
      align_ID_string(ID_example, ID_string);  
      uint8_t public_key_uint[32]; 
      memcpy(public_key_uint, public_key, strlen(public_key)+1);
      AES_encrypt(public_key_uint,ID_string); 
      Serial.println("ana hena yabona");
      uint16_t size_yabona = 64; 
      Serial.println(strlen(ID_string));
      for (int i = 0; i < len; i++) Serial.print(ID_string[i]);
      send_packet(size_yabona, ID_string, ID_HEADER);
      
    } 
    else if(buf[0]==ACK_ID && flag_ID_ack_done == true)
    {
        Serial.println("The database received the key packet");//add code for what to do with data in case it is a public key
    
    }
   
}


void send_packet(uint16_t packetSize, char * packet_to_send, Packet_Header packet_header_to_send) {
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

void send_packet(uint16_t packetSize, uint8_t packet_to_send[64], Packet_Header packet_header_to_send) {
  if (packetSize > MAX_BUFFER_SIZE) {
    packetSize = MAX_BUFFER_SIZE;
  }
  Serial.println("men gowa: ");
  for (int i = 0; i < packetSize; i++) Serial.print(packet_to_send[i]);
  Tiny::Packet<64> packet; 
  proto_door.enableCheckSum(); 
  proto_door.beginToSerial();
  packet.clear(); 
  Serial.println();
  packet.put(packet_header_to_send);          //add the packet header as the first byte
  char packet_to_send_char[64]; 
  uint8_t packet_to_send_int[64]; 
  memcpy(packet_to_send_char, packet_to_send, strlen(packet_to_send)+1);
  Serial.println("packet_to_send_char: ");
  Serial.println(packet_to_send_char);
  Serial.println("packet_to_send_int: ");
  memcpy(packet_to_send_int, packet_to_send_char, strlen(packet_to_send_char)+1);
  for (int i = 0; i < packetSize; i++) Serial.print(packet_to_send_int[i]);
  for (int i = 0; i < packetSize; i++) packet.put(packet_to_send_char[i]);
  Serial.println("ba3d el put: ");
  for (int i = 0; i < packetSize; i++) Serial.print(packet_to_send[i]);
  proto_door.write(packet);
  Serial.println("ba3d el write: ");
  for (int i = 0; i < packetSize; i++) Serial.print(packet_to_send[i]);
  
}