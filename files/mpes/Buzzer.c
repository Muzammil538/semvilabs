#include<lpc214x.h>
#include<stdio.h>

volatile unsigned int delay;

void MyDelay(void){
  int i;
  for(i = 0; i < 100000; i++);
  for(delay = 0; delay < 100000; delay++);
}

int main(void){
  IOODIR = 0x02000000;
  while(1){
    IOSET = 0x02000000;
    MyDelay();
    IOCLR = 0x02000000;
    MyDelay();
  }
}