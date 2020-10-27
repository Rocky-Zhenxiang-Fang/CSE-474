/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This file is used to turn on and off onboard LEDs in periodic pattern
 * */

#include <stdint.h>
#include <stdio.h>
#include "Task_1_a_header.h"

int main(void) {
  SetUp(); 
  SetLEDs();
  while (1) {
    TurnOnOneByOne(); 
    TurnOffOneByOne(); 
  }
  return 0;
}

void SetUp(void) {
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020; // Enable PortF(led3, 4) and PortN(led1, 2) GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
}

void SetLEDs(void) {
  GPIODIR_F = 0x11; // Set PF0, PF4 to output
  GPIODEN_F = 0x11; // Set PF0, PF4 to digital port
  GPIODIR_N = 0x3; // Set PN1, PN0 to output
  GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
  GPIODATA_N_1 = 0x0;
  GPIODATA_N_0 = 0x0;
  GPIODATA_F_0 = 0x0;
  GPIODATA_F_4 = 0x0; // Turn off all LEDs
}

void Delay(void) {
  for (unsigned long j = 0; j < 1000000; j++) { }
}

void TurnOnOneByOne(void) {
  GPIODATA_N_1 = 0x2;
  Delay();
  GPIODATA_N_0 = 0x1;
  Delay();
  GPIODATA_F_4 = 0x10;
  Delay();
  GPIODATA_F_0 = 0x1;
  Delay();
}

void TurnOffOneByOne(void) {
  GPIODATA_N_1 = 0x0;
  Delay();
  GPIODATA_N_0 = 0x0;
  Delay();
  GPIODATA_F_4 = 0x0;
  Delay();
  GPIODATA_F_0 = 0x0;
  Delay();
}
