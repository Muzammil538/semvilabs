#include <LPC214x.h>
volatile unsigned int delay;
int main (void)
{
PINSEL2 = (PINSEL2 &~ 0x0000FFFF) ; // Configure P1.16 to P1.23 as GPIO
IO1DIR = (IO1DIR | 0x00FF0000) ; // Configure P1.16 to P1.23 as Output
while(1)
{
IO1CLR = (IO1CLR | 0x00FF0000); // CLEAR (0) P0.10 to P0.13 and P0.18
to P0.21, LEDs ON
for(delay=0; delay<350000; delay++); // delay
IO1SET |=0x00FF0000;// SET (1) P0.10 to P0.13 and P0.18 to P0.21, LEDs
OFF
for(delay=0; delay<350000; delay++); // delay
}
}