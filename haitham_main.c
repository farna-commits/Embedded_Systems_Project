
#include <ArduinoJson.h>
#include <Key.h>
#include <Keypad.h>
#include <stdint.h>


const uint8_t ID_SIZE      = 4;
const uint8_t PIN_SIZE     = 4;

Keypad  map_keypad()
{
char key;
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
return keypad;
}

void read_ID(uint16_t array_ID[], Keypad keypad) {
  int i=0;
  while (i<ID_SIZE)
  {
    key = keypad.getKey();
    if (key!= NO_KEY){
      array_ID[i] = key;
      i++;
    }
  }
}

StaticJsonDocument load_database()
{
    StaticJsonDocument<700> doc;
    // char json[] = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900, 2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";
    char json[] = "{\"ID\": [2606, 3405, 2303, 2073, 1635, 3668, 1133, 1968, 1440, 2806, 3773, 2220, 3912, 2251, 3870, 2362, 1236, 3161, 1593, 3211, 2797, 1388, 1272, 3016, 2290, 1768, 2186, 1928, 2013, 2561, 1689, 3988, 1182, 2061, 1848, 2305, 2053, 3479, 2506, 2032, 3678, 2472, 3909, 3137, 3645, 2761, 2247, 2899, 2270, 1900]}";
    while (!Serial) continue;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }  

  return doc;

}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  Keypad keypad;
  uint16_t array_ID[ID_SIZE]    = {};
  uint8_t temp = database["ID"][49];
  StaticJsonDocument<700> database;


  keypad=map_keypad();
  read_ID(array_ID, keypad);
  database=load_database();

  Serial.println(temp);

  
}

void loop() {
//   int temp = doc["ID"][2];
//  Serial.println("test");
//   put your main code here, to run repeatedly:
//   read_ID();
//   Serial.println("test9");
//   Serial.println(array_ID[0]);
//  Serial.println(array_ID[1]);
//  Serial.println(array_ID[2]);
//   Serial.println(array_ID[3]);
 
}
