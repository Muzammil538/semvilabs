#include <lpc214x.h>

#define PHASE A 0x90000000;
#define PHASE B 0x50000000;
#define PHASE C 0x60000000;
#define PHASE D 0xA0000000;

unsigned int delay;

int main(void){
  PINSEL1 = PINSEL1 & ~0xFF000000;
  IODIR = IOODIR | 0XF00000000;
  IOSET = IOSET | 0XF0000000;
  while (1)
  {
    IOOSET = PHASE D;
    IOOCLR = (~PHASE D) & 0XF0000000;
    for (delay = 0; delay < 100000; delay++);
    IOOSET = PHASE C;
    IOOCLR = (~PHASE C) & 0XF0000000;
    for (delay = 0; delay < 100000; delay++);
    IOOSET = PHASE B;
    IOOCLR = (~PHASE B) & 0XF0000000;
    for (delay = 0; delay < 100000; delay++);
    IOOSET = PHASE A;
    IOOCLR = (~PHASE A) & 0XF0000000;
    for (delay = 0; delay < 100000; delay++);
  }
  
}