#include "methods.h"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"

//Tiny objects 
Tiny::ProtoHd  proto_database (proto_buffer_database, sizeof(proto_buffer_database),  onFrameIn_database);
Tiny::ProtoHd  proto_door     (proto_buffer_door,     sizeof(proto_buffer_door),      onFrameIn_door);


static bool __check_ID (char * hashed_ID) {
  char * origin; 
  origin = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char)); 
  for (int i = 0; i < DB_SIZE; i++)
  {
    origin = doc["ID"][i]; 
    Print("Origin: ");
    Println(origin);
    if (strcmp(hashed_ID, origin) == 0)
      return true; 
  }
  return false; 
    
}


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
    Print(F("deserializeJson() failed: "));
    Println(error.f_str());
  }  
  return error; 
}

static void __align_ID_string(int ID_example, char a[16]) {
  itoa(ID_example, a, 10);
  strcat(a,"00000000000");
}

static void __array2uint16(uint16_t &a, uint16_t array[ID_SIZE]) {
  for (int i = 0; i < ID_SIZE; i++)
    {
      a = a * 10; 
      a = a + (array[i] - 48); 
    }
}

void AES_encrypt(uint8_t * key, char * buf ) {
  aes128_enc_single(key, buf);
  Print("Encrypted ID:");
  Println(buf);

}

void AES_decrypt(uint8_t * key, char * buf) {
  aes128_dec_single(key, buf);
}



void onFrameIn_database(char *buf, int len) {

  //Case key 
  if(buf[0] == DIFFIE_PUBLIC_KEY && flag_key_done == false) {

    //Printing key packet 
    Print("The received key packet from the door is: "); 
    for (int i=0; i<len; i++) Print(buf[i]);
    
    //DH1 
    DH1(public_key_database, secret_key_database); 
    Println("this is the database PK: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database[i]);
    Println();
    Println("this is the database SK: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(secret_key_database[i]);
    Println();

    //Extracting key from packet  
    for (int i = 0; i < KEY_SIZE; i++) public_key_door_copy[i] = buf[i+2];
    Print("The received public key copy is: "); 
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door_copy[i]); Println();
    
    //Send packet 
    flag_key_done = true; 
    send_packet_database(KEY_SIZE, public_key_database, ACK_KEY);

  }
  //Case ID 
  else if(buf[0]== ID_HEADER && flag_key_done == true) {

    //Printing ID packet 
    Print("The received Encrypted ID packet is: "); 
    for (int i=0; i<len; i++) Print(buf[i]); Println();

    //DH2
    DH2(public_key_door_copy, secret_key_database);
    Println("ABG key is: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door_copy[i]); Println();

    //Extracting ID packet 
    char * buf_copy;
    buf_copy = (char*)calloc(len, sizeof(char)); 
    for (int i = 0; i < len - 1; i++) buf_copy[i] = buf[i+2];
    Print("The encrypted ID copy is: "); 
    for (int i = 0; i < len - 1; i++) Print(buf_copy[i]); Println();


    //Decryption        
    AES_decrypt(public_key_door_copy, buf_copy);
    //Copy decrypted string into a new sized string 
    for (int i = 0; i < ID_SIZE; i++) decrypted_string[i] = buf_copy[i];
    Print("Decrypted ID: "); 
    for (int i = 0; i < ID_SIZE; i++) Print(decrypted_string[i]);
  

    //Send done 
    Tiny::Packet<64> packet;
    packet.clear();
    packet.put( "Now checking: " );          
    proto_database.write(packet);

    //Hashing 
    char * hashed_string; 
    hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));     
    ProcessInputMessage(decrypted_string, hashed_string);
    Print("Hashed printing from main method: ");  
    Println(hashed_string);  

    Println("Debug: ");
    if (!flag_call_once) {
      Read_json(doc,json);
      flag_call_once = true; 
    }
    
    if(__check_ID(hashed_string)) {
      // flag_response_done = true; 
      // Print("Flag now before send packet: ");
      // Println(flag_response_done);
      flag_key_done = false; 
      hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));   
      send_packet_database(KEY_SIZE, "Access Granted, Open Door", ACK_ACCESS);
      Print("Flag now after send packet: ");
      Println(flag_response_done);
      
    }
    else {
      // flag_response_done = true; 
      flag_key_done = false; 
      hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));   
      send_packet_database(KEY_SIZE, "Access Denied", ACK_ACCESS);
    }
    

  }
}

void onFrameIn_door(char *buf, int len) {
  if(buf[0]==ACK_KEY)
  {
    //Printing Key packet 
    Print("The received key packet from the database is: "); 
    for (int i=0; i<len; i++) Print(buf[i]); Println();

    //Extracting key from packet  
    for (int i = 0; i < KEY_SIZE; i++) public_key_database_copy[i] = buf[i+2];
    Print("The received public key copy is: "); 
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database_copy[i]); Println();


    //Fetch from example
    int ID_example = 0; 
    uint16_t Entered_ID = 0;
    __array2uint16(Entered_ID, array_ID);
    Println("ID: ");
    Println(Entered_ID);
    ID_example = Entered_ID; 
    Print("Fetching an ID from database as an example: ");
    Println(ID_example);  
    char * ID_string;
    ID_string = (char*)calloc(64, sizeof(char));  
    __align_ID_string(ID_example, ID_string); 
    //After Alignment
    Println("Aligned: ");
    Println(ID_string);

    //DH2
    DH2(public_key_database_copy, secret_key_door);
    Println("ABG key is: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database_copy[i]); Println();
    Print("ABG kolo is: ");
    Println((int)public_key_database_copy);

    //Print the encrypted data
    Println("The encrypted ID before encryption is: ");
    for (int i = 0; i < len; i++) Print(ID_string[i]); Println();

    //Encryption     
    AES_encrypt(public_key_database_copy, ID_string);

    //Print the encrypted data
    Println("The encrypted ID is: ");
    for (int i = 0; i < len; i++) Print(ID_string[i]); Println();

    // Send the encrypted data
    flag_ID_ack_done = true;
    send_packet_door(KEY_SIZE, ID_string, ID_HEADER);
  }
  else if (buf[0]==ACK_ACCESS && flag_ID_ack_done == true){
    //Decision to open door or not
    for (int i = 1; i < len; i++) Print(buf[i]); Println();
    flag_response_done = true; 
    Print("Flag now: ");
    Println(flag_response_done);
    
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