#include <stdint.h>
#include "lab1.h"

void TurnOff(void) {
  volatile unsigned short delay = 0;
 RCGCGPIO |= 0x1020; // Enable PortF(led3, 4) and PortN(led1, 2) GPIO
 delay++; // Delay 2 more cycles before access Timer registers
 delay++; // Refer to Page. 756 of Datasheet for info
 GPIODIR_F = 0x11; // Set PF0, PF4 to output
 GPIODEN_F = 0x11; // Set PF0, PF4 to digital port
 GPIODIR_N = 0x3; // Set PN1, PN0 to output
 GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
 GPIODATA_N_1 = 0x0;
 GPIODATA_N_0 = 0x0;
 GPIODATA_F_0 = 0x0;
 GPIODATA_F_4 = 0x0; // Turn off all LEDs
}


int main(void) {
 TurnOff();
 while (1) {

 }
 return 0;
}
