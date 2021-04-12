//Includes 
#include <Key.h>
#include <Keypad.h>
#include <stdint.h>

//Globals 
//Keypad

const uint8_t ID_SIZE      = 4;
const uint8_t PIN_SIZE     = 4;

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
byte rowPins[COLS]                  = {6,7,8,9}; //connect to the column pinouts of the keypad
Keypad keypad                       = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

bool flag_key = false;

char array_ID[ID_SIZE]    = {};
char array_PIN[PIN_SIZE]  = {};