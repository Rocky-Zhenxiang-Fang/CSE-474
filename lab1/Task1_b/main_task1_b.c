/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This file is used to control onboard LEDs using onboard switch
 * */

#include <stdint.h>
#include <stdio.h>
#include "Task_1_b_header.h"

int main(void) {
   SetUp(); 
   SetLEDs();
   SetSwitches();
   while (1) {
      LightLED();
   }
   return 0;
}

void SetUp(void) {
   volatile unsigned short delay = 0;
   RCGCGPIO |= 0x1100; // Enable PortJ(Switches) and PortN(led1, 2) GPIO
   delay++; // Delay 2 more cycles before access Timer registers
   delay++; // Refer to Page. 756 of Datasheet for info
}

void SetSwitches(void) {
   GPIODIR_J = 0x0; // Set all PJ pins to input
   GPIOLOCK_J = 0x4C4F434B; // Unlock Port P
   GPIOCR_J = 0x3; // Allows GPIOPUR and GPIODEN to be used
   GPIOPUR_J = 0x3; // Select PJ0, PJ1 to pull up
   GPIODEN_J = 0x3; // Set PJ0 and PJ1 to digital port
}


void SetLEDs(void) {
   GPIODIR_N = 0x3; // Set PN1, PN0 to output
   GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
   GPIODATA_N_1 = 0x0;
   GPIODATA_N_0 = 0x0;
}

void LightLED(void) {
   if (GPIODATA_J == 0x1) { // Switch_2 is pressed
      GPIODATA_N_1 = 0x0;
      GPIODATA_N_0 = 0x1; // Turn on LED2
   }
   else if (GPIODATA_J == 0x2) { // Switch_1 is pressed
      GPIODATA_N_1 = 0x2; // Turn on LED1
      GPIODATA_N_0 = 0x0;
   }
   else if (GPIODATA_J == 0x0) { // Both switch is pressed
      GPIODATA_N_1 = 0x2;
      GPIODATA_N_0 = 0x1; // Turn on both LEDs
   }
   else { // None switch is pressed
      GPIODATA_N_1 = 0x0;
      GPIODATA_N_0 = 0x0;
   }
}


