/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This file is used to turn on and off onboard LEDs in periodic pattern
 * */

#include <stdint.h>
#include <stdio.h>
#include "task2_a_header.h"

unsigned char flag = 0;

int main(void) {
  SetUpGPIO();
  SetUpTimer();
  while (1) {
    SwitchLEDs();
  }
  return 0;
}

void SetUpGPIO(void) {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020; // Enable PortF(led3, 4) and PortN(led1, 2) GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
  GPIODIR_F = 0x11; // Set PF0, PF4 to output
  GPIODEN_F = 0x11; // Set PF0, PF4 to digital port
  GPIODIR_N = 0x3; // Set PN1, PN0 to output
  GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
  GPIODATA_N = 0x0;
  GPIODATA_F = 0x0;  // Turn off all LEDs
}

void SetUpTimer(void) {
   //step 1
   RCGCTIMER |= (1<<0); // Enables GPtimer 0, sets 1 at field 0

   //step 2
   GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
   GPTMCTL_TIMER_0 &= ~(1<<8); //Disables timer 0_B, set 0 to field 8

   //step 3
   GPTMCFG_TIMER_0 = 0x00000000; 

   //step 4
   GPTMCFG_TIMER_0 = 0x0; // select 32-bit mode

   //step 5
   GPTMTAMR_TIMER_0 = (0x2<<0); 

   //step 6
   GPTMTAMR_TIMER_0 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4

   //step 7
   GPTMTAILR_TIMER_0 = 0x00F42400; // 16000000 

   //step 8, Enable interupt
   GPTIMR_TIMER_0 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   NVIC_EN0 |= (0x1<<19); // Enables timer_0 interrupt. Set field 19 to 1

   //step 9
   GPTMCTL_TIMER_0 |= (1<<0); // Enables timer 0_A
}

void Timer0A_Handler( void ){
  GPTMICR_TIMER_0 |= (1<<0); // Clear interupt flag
  if (flag != 7) {
     flag += 1; 
  }
  else {
     flag = 0; 
  }
}

void SwitchLEDs(void) {
   switch (flag) {
      case 0:
         GPIODATA_N |= LED1;  //switch the state of LED1, on PN1
         break;

      case 1:
         GPIODATA_N |= LED2;  //switch the state of LED2, on PN0
         break;

      case 2:
         GPIODATA_F |= LED3;  //switch the state of LED3, on PF4
         break;

      case 3:
         GPIODATA_F |= LED4;  //switch the state of LED4, on PF0
         break;

      case 4:
         GPIODATA_N &= ~LED1;  //switch the state of LED1, on PN1
         break;

      case 5:
         GPIODATA_N &= ~LED2;  //switch the state of LED2, on PN0
         break;

      case 6:
         GPIODATA_F &= ~LED3;  //switch the state of LED3, on PF4
         break;

      case 7:
         GPIODATA_F &= ~LED4;  //switch the state of LED4, on PF0
         break;

      default:
         break;
   }
}
