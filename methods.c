//Includes 
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\methods.h"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"

//Statics 
static bool __check_ID (char * hashed_ID) {
  char * origin; 
  origin = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char)); 
  Println("Begining Linear Search in Hashed Database: ");
  for (int i = 0; i < DB_SIZE; i++)
  {
    origin = doc["ID"][i]; 
    Print("Database key number ["); Print(i); Print("] : ");
    Println(origin);
    if (strcmp(hashed_ID, origin) == 0) {
      Println("Match Found!");
      return true; 
    }
  }
  return false; 
    
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

static void __AES_encrypt(uint8_t * key, char * buf ) {
  aes128_enc_single(key, buf);
  Print("Encrypted ID: ");
  Println(buf);

}

static void __AES_decrypt(uint8_t * key, char * buf) {
  aes128_dec_single(key, buf);
}

static void __onFrameIn_database(char *buf, int len) {

  //Case key 
  if(buf[0] == DIFFIE_PUBLIC_KEY && flag_key_done == false) {
        
    //DH1 
    DH1(public_key_database, secret_key_database); 
    Print("Database Public Key: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database[i]);
    Println();
    Print("Database Secret Key: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(secret_key_database[i]);
    Println();

    //Printing key packet 
    Print("The Received Key Packet From The Door is: "); 
    for (int i=0; i<len; i++) Print(buf[i]); Println();

    //Extracting key from packet  
    for (int i = 0; i < KEY_SIZE; i++) public_key_door_copy[i] = buf[i+2];
    Print("The Received Public Key is: "); 
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door_copy[i]); Println();
    
    //Send packet 
    flag_key_done = true; 
    send_packet_database(KEY_SIZE, public_key_database, ACK_KEY);

  }
  //Case ID 
  else if(buf[0]== ID_HEADER && flag_key_done == true) {

    //DH2
    DH2(public_key_door_copy, secret_key_database);
    Print("Common Generated Key in Door and Database is: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_door_copy[i]); Println();


    //Printing ID packet 
    Print("The Received Encrypted ID Packet is: "); 
    for (int i=0; i<len; i++) Print(buf[i]); Println();

    //Extracting ID packet 
    char * buf_copy;
    buf_copy = (char*)calloc(len, sizeof(char)); 
    for (int i = 0; i < len - 1; i++) buf_copy[i] = buf[i+2];
    Print("The Encrypted ID is: "); 
    for (int i = 0; i < len - 1; i++) Print(buf_copy[i]); Println();

    //Decryption        
    __AES_decrypt(public_key_door_copy, buf_copy);
    //Copy decrypted string into a new sized string 
    for (int i = 0; i < ID_SIZE; i++) decrypted_string[i] = buf_copy[i];
    Print("Decrypted ID: "); 
    for (int i = 0; i < ID_SIZE; i++) Print(decrypted_string[i]); Println();
  

    //Hashing 
    Println("Hashing Decrypted ID....."); 
    char * hashed_string; 
    hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));     
    ProcessInputMessage(decrypted_string, hashed_string);

    //Deserialize JSON once 
    if (!flag_call_once) {
      Read_json(doc,json);
      flag_call_once = true; 
    }

    //Check with database 
    if(__check_ID(hashed_string)) {
      flag_key_done = false; 
      hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));   
      send_packet_database(KEY_SIZE, "Access Granted, Open Door", ACK_ACCESS);      
    }
    else {
      flag_key_done = false; 
      hashed_string = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));   
      send_packet_database(KEY_SIZE, "Access Denied", ACK_ACCESS);
    }
    
    Println();

  }
}

static void __onFrameIn_door(char *buf, int len) {
  if(buf[0]==ACK_KEY)
  {
    //Printing Key packet 
    Println();
    Print("The Received Key Packet From The Database is: "); 
    for (int i=0; i<len; i++) Print(buf[i]); Println();

    //Extracting key from packet  
    for (int i = 0; i < KEY_SIZE; i++) public_key_database_copy[i] = buf[i+2];
    Print("The Received Public Key is: "); 
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database_copy[i]); Println();


    //Fetch from example
    int ID_example = 0; 
    uint16_t Entered_ID = 0;
    __array2uint16(Entered_ID, array_ID);
    ID_example = Entered_ID; 
    char * ID_string;
    ID_string = (char*)calloc(64, sizeof(char));  
    __align_ID_string(ID_example, ID_string); 

    //DH2
    DH2(public_key_database_copy, secret_key_door);
    Print("Common Generated Key in Door and Database is: ");
    for (int i = 0; i < KEY_SIZE; i++) Print(public_key_database_copy[i]); Println();

    //Encryption     
    __AES_encrypt(public_key_database_copy, ID_string);

    // Send the encrypted data
    flag_ID_ack_done = true;
    send_packet_door(KEY_SIZE, ID_string, ID_HEADER);
    Println();
  }
  else if (buf[0]==ACK_ACCESS && flag_ID_ack_done == true){
    //Decision to open door or not
    for (int i = 1; i < len; i++) Print(buf[i]); Println();
    flag_response_done = true; 
    Println();
    Println();
    Println("Enter ID: ");   
  }
}

//Publics 
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
