//Includes 
#include <Key.h>
#include <Keypad.h>
#include <stdint.h>
#include <ArduinoJson.h>
#include <Curve25519.h>
#include <AESLib.h>
#include <TinyProtocol.h>
// #include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\hashing.c"
// #include "includes.h"

#ifdef __cplusplus
 extern "C" {
#endif

//Defines 
#define DH1(X, Y)    Curve25519::dh1(X, Y)
#define DH2(X, Y)    Curve25519::dh2(X, Y)
#define Println(a)   (Serial.println(a))
#define Print(a)     (Serial.print(a))

//Enum
enum Packet_Header: char {
  ID_HEADER         = 'h',
  DIFFIE_PUBLIC_KEY = 'p',
  ACK_ID            = 'R', 
  ACK_KEY           = 'K'
};

//Variables 
const int               ID_SIZE                                   = 4;
const byte              ROWS                                      = 4;
const byte              COLS                                      = 4; 
const uint8_t           KEY_SIZE                                  = 32;
const int               DB_SIZE                                   = 50;   
const uint16_t          MAX_BUFFER_SIZE                           = 256; 
char                    key2;                       
char                    keys[ROWS][COLS]                          = {{'1','2','3','L'}, {'4','5','6','X'}, {'7','8','9','S'}, {'M','0','C','E'}};
byte                    rowPins[ROWS]                             = {6,7,8,9};
byte                    colPins[COLS]                             = {2,3,4,5};
Keypad                  keypad                                    = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
StaticJsonDocument<700> doc;                        
char                    json[]                                    = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";
uint16_t                array_ID[ID_SIZE]                         = {};
uint8_t                 proto_buffer_door     [MAX_BUFFER_SIZE]; 
uint8_t                 proto_buffer_database [MAX_BUFFER_SIZE]; 
bool                    flag_key_done                             = false;
bool                    flag_ID_ack_done                          = false;
bool                    flag_keep_sending_ID                      = false;
uint8_t*                public_key_database                       = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
uint8_t*                secret_key_database                       = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
uint8_t*                public_key_door                           = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
uint8_t*                secret_key_door                           = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
uint8_t*                public_key_door_copy                      = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
uint8_t*                public_key_database_copy                  = (uint8_t*)calloc(KEY_SIZE, sizeof(uint8_t));
char   *                decrypted_string                          = (char*)   calloc(ID_SIZE , sizeof(char));

//Functions 
DeserializationError    Read_json             (StaticJsonDocument<700>, char*); 
void                    AES_encrypt           (uint8_t * key, uint8_t * buf );
void                    AES_decrypt           (uint8_t * key, char * buf); 
void                    onFrameIn_database    (char *buf, int len); 
void                    onFrameIn_door        (char *buf, int len); 
void                    send_packet_door      (uint16_t, char *, Packet_Header); 
void                    send_packet_database  (uint16_t, char *, Packet_Header); 
bool                    check_ID              (char hashed_ID[]);
static void             __array2uint16        (uint16_t &a, uint16_t array[ID_SIZE]);
static void             __align_ID_string     (int, char a[]);



#ifdef __cplusplus
}
#endif