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
const int               DB_SIZE                                   = 25;   
const uint16_t          MAX_BUFFER_SIZE                           = 256; 
char                    key2;                       
char                    keys[ROWS][COLS]                          = {{'1','2','3','L'}, {'4','5','6','X'}, {'7','8','9','S'}, {'M','0','C','E'}};
byte                    rowPins[ROWS]                             = {6,7,8,9};
byte                    colPins[COLS]                             = {2,3,4,5};
Keypad                  keypad                                    = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
StaticJsonDocument<700> doc;                        
//char                    json[]                                    = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";
char                    json[]                                    = "{\"ID\": ['b028e7b30ae6629575651e0369e15c482a1b97a7d437334f1b1cf00f5c06a78f', 'f8eebb16fad8156c037189e6940ed46ddc859410c65a31e17a91512622379a90', 'dc7b81fd2fff7f765c9db9420b55a09a31f1ec49492aab0ebc8739ba5c20312f', 'd511b5d469027daccddb924368c9069473495e0da8b42f1a0c4283fd68c770d3', '735e4a8aa7691b6d7e64836a10faa46e2246aadb16addbf59db7f7b0f45e6d19', '9fed0cec52a189e77e418dd123d61e03eaffbdf3910deccbd746b8c91804ca71', '7a99d42d79e9bafeaa5ccedaf0135267da4ccd197a99131a8cf15025cb54ab18', 'a48622b535728587fd351763d1296c7ec9cb5bc6743d5f22b011d5b5c3ef688f', 'a4ff3ad278c7b0575a8e32ae08eebfb327e37762cb1a2caa793c431b9b2ca2c6', '23e3ca332c61648a9e8ebb917ceb2f63bc45b7b33b4769b05dd86b028d56c63c', '3ca74779bdf0f0ba587f6a5dd2714505248a84002b1aeda3e63646fe0ed4b4fe', '230ad27c6e6e27669363918994e83fc533cf8d5ab4ee57ea1855428b8c553a53', '8664f0d168799ba7eb959cb75d0c173af2e4b197b40468b725460bb5d34c9d5f', '5a9788c1434d2653d2d6ed7eda409a5af5d196f29b9da89c52ff83a94490445f', '0228374d12ee995cdeff8e25d819990f80d40c8773321fafe4ce1572c7df29af', '9bf42f4b66fe462d800a8b659ec866dca7f23597393f9cb25456d41f5458b590', '7b0838c2af7e6b1f3fe5a49c32dd459d997a931cee349ca6869f3c17cc838394', 'fbfd6393c2b37ff4ef7c9fd72aa259d6651e064a98825c102cc296c53fc00e54', '38813d9a6e5332787f13dac070d265339c4c711beea8b21f82966684bda30e79', '4a8ed6268c1741e4d3b85f8e40aeddc34d35a965671eaa5207abd4cdef049d6f', 'd6861565a3d4e63d2b5bab69bf5bb8c0011980377f855c94ac2798ac7919b5f6', 'e401f2bd399f3456e5348217a7908ca545ea6d179f60f297a1b0133e87d2ff85', 'c676b6e9a4ca6b5ac36368e46b51915a0fcf7371a90c9155ddfa82195555496a', 'dee71b933e851f0080386652c75645778d2804d4c595adb9978355a73461aabc', '808c9cfebd38d4d93da5bb3b32be7e39cbb403672c5ce39b83bffe7d5660b114']}";
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
char   *                decrypted_string                          = (char*)   calloc(KEY_SIZE, sizeof(char));

//Functions 
DeserializationError    Read_json             (StaticJsonDocument<700>, char*); 
void                    AES_encrypt           (uint8_t * key, uint8_t * buf );
void                    AES_decrypt           (uint8_t * key, char * buf); 
void                    onFrameIn_database    (char *buf, int len); 
void                    onFrameIn_door        (char *buf, int len); 
void                    send_packet_door      (uint16_t, char *, Packet_Header); 
void                    send_packet_database  (uint16_t, char *, Packet_Header); 
bool                    check_ID              (char *);
static void             __array2uint16        (uint16_t &a, uint16_t array[ID_SIZE]);
static void             __align_ID_string     (int, char a[]);



#ifdef __cplusplus
}
#endif