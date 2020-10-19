#include <stdint.h>
#include "lab1.h"

void SetLED4(void) {
 GPIODIR_F = 0x1; // Set PF0 to output
 GPIODEN_F = 0x1; // Set PF0 to digital port
}

void SetSwitches(void) {
  GPIODIR_J = 0x0; // Set all PJ pins to input
  GPIOLOCK_J = 0x4C4F434B; // Unlock Port P
  GPIOCR_J = 0x3; // Allows GPIOPUR and GPIODEN to be used
  GPIOPUR_J = 0x1; // Select PJ0 to pull up
  GPIODEN_J = 0x3; // Set PJ0 and PJ1 to digital port
}

int main(void) {
 volatile unsigned short delay = 0;
 RCGCGPIO |= 0x2020; // Enable PortF(led4) and PortP(switches) GPIO
 delay++; // Delay 2 more cycles before access Timer registers
 delay++; // Refer to Page. 756 of Datasheet for info
 SetLED4();
 SetSwitches();
 while (1) {
   while (GPIODATA_J == 0x1) {
      GPIODATA_F = 0x0;
   }
    GPIODATA_F = 0x1;
 }
 return 0;
}
