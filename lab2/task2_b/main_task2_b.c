#include <stdint.h>
#include <stdio.h>
#include "task2_b_header.h"


int main(void) {
   SetUpGPIO(); 
   SetUpTimer();
   while (1) {
     
   }
   return 0;
}

void SetUpGPIO(void) {
   volatile unsigned short delay = 0;
   RCGCGPIO |= 0x1100; // Enable PortJ(Switches) and PortN(led1, 2) GPIO
   delay++; // Delay 2 more cycles before access Timer registers
   delay++; // Refer to Page. 756 of Datasheet for info
   GPIODIR_N = 0x3; // Set PN1, PN0 to output
   GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
   GPIODATA_N = 0x0;

   GPIODIR_J = 0x0; // Set all PJ pins to input
   GPIOLOCK_J = 0x4C4F434B; // Unlock Port J
   GPIOCR_J = 0x3; // Allows GPIOPUR and GPIODEN to be used
   GPIOPUR_J = 0x3; // Select PJ0, PJ1 to pull up
   GPIODEN_J = 0x3; // Set PJ0 and PJ1 to digital port

   // Prot_J (Switches) Interrupt
   GPIOIS_J &= ~0x3; // make switches edge sensitive
   GPIOIBE_J &= ~0x3; // Only detect one edge, selected in IEV
   GPIOIEV_J = ~0x3; // detect on falling edge
   GPIOICR_J |= 0x3; // clear prior interrupt
   GPIOIM_J |= 0x3; // unmask interrupt
   NVIC_EN1 |= (0x1 << 19); // Enables Port I interrupt. Set field 19 to 1
};

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
   GPTMIMR_TIMER_0 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   NVIC_EN0 |= (0x1<<19); // Enables timer_0 interrupt. Set field 19 to 1

   //step 9
   GPTMCTL_TIMER_0 |= (1<<0); // Enables timer 0_A
}

void Timer0A_Handler(void) {
   GPTMICR_TIMER_0 |= (1<<0); // Clear interupt flag
   GPIODATA_N ^= (0x1 << LED1);  //switch the state of LED1, on PN1
}

void PortJ_Handler(void) {
   if (((GPIOMIS_J & (1 << SW1)) >> SW1) == 0x1) { // Sw1 is pressed
      GPIOICR_J |= 0x1 << SW1; // clear interupt from SW1
      GPIODATA_N |= 0x1 << LED2; // light LED2
      GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
   }

   if (((GPIOMIS_J & (1 << SW2)) >> SW2) == 0x1) { // Sw2 is pressed
      GPIOICR_J |= 0x1 << SW2; // clear interupt from SW2
      GPIODATA_N &= ~(0x1 << LED2); // dim LED2
      GPTMCTL_TIMER_0 |= (1<<0); // Reenables timer 0_A 
   }
}