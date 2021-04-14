#include "keypad_new.h"

void read_ID() {
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

void read_PIN() {
  int i=0;
  while (i<PIN_SIZE)
  {
    key = keypad.getKey();
    if (key!= NO_KEY){
      array_PIN[i] = key;
      i++;
    }
  }
}
