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
const int               DB_SIZE                                   = 8;   
const uint16_t          MAX_BUFFER_SIZE                           = 256; 
char                    key2;                       
char                    keys[ROWS][COLS]                          = {{'1','2','3','L'}, {'4','5','6','X'}, {'7','8','9','S'}, {'M','0','C','E'}};
byte                    rowPins[ROWS]                             = {6,7,8,9};
byte                    colPins[COLS]                             = {2,3,4,5};
Keypad                  keypad                                    = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
StaticJsonDocument<700> doc;                        
//char                    json[]                                    = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";
// char                    json[]                                    = "{\"ID\": [b028e7b30ae6629575651e0369e15c482a1b97a7d437334f1b1cf00f5c06a78f, f8eebb16fad8156c037189e6940ed46ddc859410c65a31e17a91512622379a90, dc7b81fd2fff7f765c9db9420b55a09a31f1ec49492aab0ebc8739ba5c20312f, d511b5d469027daccddb924368c9069473495e0da8b42f1a0c4283fd68c770d3, 735e4a8aa7691b6d7e64836a10faa46e2246aadb16addbf59db7f7b0f45e6d19, 9fed0cec52a189e77e418dd123d61e03eaffbdf3910deccbd746b8c91804ca71, 7a99d42d79e9bafeaa5ccedaf0135267da4ccd197a99131a8cf15025cb54ab18, a48622b535728587fd351763d1296c7ec9cb5bc6743d5f22b011d5b5c3ef688f, a4ff3ad278c7b0575a8e32ae08eebfb327e37762cb1a2caa793c431b9b2ca2c6, 23e3ca332c61648a9e8ebb917ceb2f63bc45b7b33b4769b05dd86b028d56c63c, 3ca74779bdf0f0ba587f6a5dd2714505248a84002b1aeda3e63646fe0ed4b4fe, 230ad27c6e6e27669363918994e83fc533cf8d5ab4ee57ea1855428b8c553a53, 8664f0d168799ba7eb959cb75d0c173af2e4b197b40468b725460bb5d34c9d5f, 5a9788c1434d2653d2d6ed7eda409a5af5d196f29b9da89c52ff83a94490445f, 0228374d12ee995cdeff8e25d819990f80d40c8773321fafe4ce1572c7df29af, 9bf42f4b66fe462d800a8b659ec866dca7f23597393f9cb25456d41f5458b590, 7b0838c2af7e6b1f3fe5a49c32dd459d997a931cee349ca6869f3c17cc838394, fbfd6393c2b37ff4ef7c9fd72aa259d6651e064a98825c102cc296c53fc00e54, 38813d9a6e5332787f13dac070d265339c4c711beea8b21f82966684bda30e79, 4a8ed6268c1741e4d3b85f8e40aeddc34d35a965671eaa5207abd4cdef049d6f, d6861565a3d4e63d2b5bab69bf5bb8c0011980377f855c94ac2798ac7919b5f6, e401f2bd399f3456e5348217a7908ca545ea6d179f60f297a1b0133e87d2ff85, c676b6e9a4ca6b5ac36368e46b51915a0fcf7371a90c9155ddfa82195555496a, dee71b933e851f0080386652c75645778d2804d4c595adb9978355a73461aabc, 808c9cfebd38d4d93da5bb3b32be7e39cbb403672c5ce39b83bffe7d5660b114, 4cbe3d6227abe57a801103c2e9a89c1ccf51eadc32c13cf5f8ebd6776fbe3671, ec3f1e361fe23b5ec31edc081aef4c1d6e9bdd184a7b92df4b7111745fa3a164, aef662afc24b5edde66ab972975340f6a9963eb766261e6bf43ffdb56b6d2a3f, 7931aa2a1bed855457d1ddf6bc06ab4406a9fba0579045a4d6ff78f9c07c440f, 5ee36f40051aec9076cd20826e9bad65d1c5fa24dc02a6bc19b83c8e7eaa4d53, 41e32284df1a73272655a26bfe6d4919ed6504972cc47461330a26e90cd9ddc3, 318c731917609d7f276fd198b928596360758b0afc5ec9e224022a2fa1403cc0, c7b96125d8bb1b5ac26f057909f1a29dcd87d00ced1a9b563f3220b6d6038d66, 7907a88d77f29a7681a0dc6c89157ea3f45959944436301085c8a34a46fe4a77, 33afd662cef42c907d9b9a13796a954d5c9675b7c6f1e3f92914df1bcc79929b, 9a5a48758da347f33bd37064cfbd05442ef005c85ae0ea4614740d5597316ed3, e5baabbb54bcea599793ede6f71485107203186274eaf8edf9d55e4f4cc581f3, ce16559af2caf7bb54bebd57a1602e29ada331b3356004265abeab0e568278cc, 6e71e71a06ff0270c038319e473f7d1beca35a3ea8015ab60e7f1ab47288d9f6, 4432cb276ffc79e796e2f86c4aabb5a223462ff45d089d30e5c78c38318c55cf, 62f4d89dd319a4e7788a88a37913e1295e46b25ba49d6741be302ff6fe0b6baf, 1ab6078431739cd9988f5c8e042389b2616911baa6c034a21b055f0b104b8527, 5c2e13bb6c475cb7e036f736084779651b362b6b9c95b4376e3cfe4550edb6a1, 1f023880f785dbb4bf7582607502366de85264636c622d0da0318a99bfeb4913, 62fe739825059f3098bcdba9e0ee2d62b18c24b8bb98dbb385551e9a04526b3c, d2cc2cd378a0cbf24293e9059fff3a2435ec802d01893fc38edf444fb9f4decc, c6d34483cedd0b1069ecae3a71aff271b3602264c67b8f46b11c324c54ad9468, cbcd59dc5044f5dab3ea5ebab2243ce0c1919976dcc707d84ddfacdd719f510b, 8e3330aeb5e96211f56a9521e80ab8b0a841921b73bc64131abee2701dff81eb, e41d64db5703c6440b5c714d57251a845bc0bd241480b41a4e7fd3e052f85a82]}";
char                    json[]                                    = "{\"ID\": ['b028e7b30ae6629575651e0369e15c482a1b97a7d437334f1b1cf00f5c06a78f', 'f8eebb16fad8156c037189e6940ed46ddc859410c65a31e17a91512622379a90', '23e3ca332c61648a9e8ebb917ceb2f63bc45b7b33b4769b05dd86b028d56c63c', 'fbfd6393c2b37ff4ef7c9fd72aa259d6651e064a98825c102cc296c53fc00e54', '4cbe3d6227abe57a801103c2e9a89c1ccf51eadc32c13cf5f8ebd6776fbe3671', '7907a88d77f29a7681a0dc6c89157ea3f45959944436301085c8a34a46fe4a77', '1ab6078431739cd9988f5c8e042389b2616911baa6c034a21b055f0b104b8527', 'e41d64db5703c6440b5c714d57251a845bc0bd241480b41a4e7fd3e052f85a82']}";
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