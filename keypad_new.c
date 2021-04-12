#include "keypad_new.h"

void read_ID() {
// for (int i = 0; i < ID_SIZE; i++)
// {
//   key = keypad.getKey();
//   if (key!= NO_KEY){
//     array_ID[i] = key;
//   }
// }

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

