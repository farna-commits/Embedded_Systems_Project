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
