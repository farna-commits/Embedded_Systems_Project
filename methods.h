//Includes 
#include <Key.h>
#include <Keypad.h>
#include <stdint.h>
#include <ArduinoJson.h>
#include <Curve25519.h>
#include <AESLib.h>

#ifdef __cplusplus
 extern "C" {
#endif

//Globals 
//Keypad

const uint8_t ID_SIZE      = 4;
const uint8_t PIN_SIZE     = 4;

char key2;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS]               = {
                                        {'1','2','3','L'},
                                        {'4','5','6','X'},
                                        {'7','8','9','S'},
                                        {'M','0','C','E'}
                                      };

byte colPins[ROWS]                  = {36,38,4,5}; //connect to the row pinouts of the keypad
byte rowPins[COLS]                  = {6,7,8,9};  //connect to the column pinouts of the keypad
Keypad keypad                       = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
StaticJsonDocument<700> doc;
char json[] = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";

//Functions 
uint16_t array_ID[ID_SIZE]    = {};
void AES_encrypt(uint8_t * key, uint8_t * buf );
DeserializationError Read_json(StaticJsonDocument<700>, char*); 
char * align_ID_string(int, char a[]);

#ifdef __cplusplus
}
#endif