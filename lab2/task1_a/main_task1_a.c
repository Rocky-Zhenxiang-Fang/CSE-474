/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This file is used to turn on and off onboard LEDs in periodic pattern
 * */

#include <stdint.h>
#include <stdio.h>
#include "task1_a_header.h"

int main(void) {
  SetUpGPIO();
  SetUpTimer();
  while (1) {
    TurnOnOneByOne(); 
    TurnOffOneByOne(); 
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
    GPTMTAMR_TIMER_0 |= (0x2<<0); 

    //step 6
    GPTMTAMR_TIMER_0 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4
  

    //step 7
    GPTMTAILR_TIMER_0 = 0x00F42400; // 16000000 

    //step 9
    GPTMCTL_TIMER_0 |= (1<<0); // Enables timer 0_A

}

void Delay(void) {
    while ((GPTMRIS_TIMER_0 & TATORIS) != 1) {}
    GPTMICR_TIMER_0 |= (1<<0);  
}

void TurnOnOneByOne(void) {
  GPIODATA_N |= LED1;
  Delay();
  GPIODATA_N |= LED2;
  Delay();
  GPIODATA_F |= LED3;
  Delay();
  GPIODATA_F |= LED4;
  Delay();
}

void TurnOffOneByOne(void) {
  GPIODATA_N &= ~LED1;
  Delay();
  GPIODATA_N &= ~LED2;
  Delay();
  GPIODATA_F &= ~LED3;
  Delay();
  GPIODATA_F &= ~LED4;
  Delay();
}
