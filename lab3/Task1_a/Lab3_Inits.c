/**
 * EE/CSE 474: Lab3 drivers starter code
 */

#include "PLL_Header.h"
#include "Lab3_Inits.h"

// STEP 0a: Include your header file here
// YOUR CUSTOM HEADER FILE HERE
#include "task1_a_header.h"

int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    MOSCCTL &= ~(0x4);                      // Power up MOSC
    MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    PLLFREQ0 |= 0x60;                       // Set MINT field
    PLLFREQ1 |= 0x4;                        // Set N field

    MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

void LED_Init(void) {
  // STEP 1: Initialize the 4 on board LEDs by initializing the corresponding
  // GPIO pins.

  // YOUR CODE HERE
   volatile unsigned short delay = 0;
   RCGCGPIO |= 0x1020; // Enable PortF(led3, 4) and PortN(led1, 2) GPIO
   delay++; // Delay 2 more cycles before access Timer registers
   delay++; // Refer to Page. 756 of Datasheet for info
   GPIODIR_F = 0x11; // Set PF0, PF4 to output
   GPIODEN_F = 0x11; // Set PF0, PF4 to digital port
   GPIODIR_N = 0x3; // Set PN1, PN0 to output
   GPIODEN_N = 0x3; // Set PN1, PN0 to digital port
   GPIODATA_N = 0x0;  // Fix me
   GPIODATA_F = 0x0;  // Turn off all LEDs, Fix me
}

void ADCReadPot_Init(void) {
   // STEP 2: Initialize ADC0 SS3.
   // 2.1: Enable the ADC0 clock
   RCGCADC |= (1 << ADC0); // set bit for ADC0 to 1 

   // 2.2: Delay for RCGCADC (Refer to page 1073)
   unsigned char delay = 0; 
   delay ++; 
   delay ++; 
   delay ++;  // delay for three system clocks

   // 2.3: Power up the PLL (if not already)
   PLLFREQ0 |= 0x00800000; // we did this for you

   // 2.4: Wait for the PLL to lock
   while (PLLSTAT != 0x1); // we did this for you

   // 2.5: Configure ADCCC to use the clock source defined by ALTCLKCFG
   ADCCC_ADC0 = 0x1; 

   // 2.6: Enable clock to the appropriate GPIO Modules (Hint: Table 15-1)
   RCGCGPIO |= (1 << 4); // Enable PortE for ADC, use PE0

   // 2.7: Delay for RCGCGPIO
   delay ++; 
   delay ++;
    
   // 2.8: Set the GPIOAFSEL bits for the ADC input pins
   GPIOAFSEL_E &= ~(1 << 0);
   GPIOAFSEL_E |= (1 << AIN2); 

   // 2.9: Clear the GPIODEN bits for the ADC input pins
   GPIODEN_E |= (1 << AIN2); 
   GPIODEN_E &= ~(1 << AIN2); 

   // 2.10: Disable the analog isolation circuit for ADC input pins (GPIOAMSEL)
   GPIOAMSEL_E |= (1 << AIN2);

   // 2.11: Disable sample sequencer 3 (SS3)
   ADCACTSS_ADC0 &= ~(1 << ASEN3); 

   // 2.12: Select timer as the trigger for SS3
   ADCEMUX_ADC0 = (0x5 << EM3); // set SS3 to timer 

   // 2.13: Select the analog input channel for SS3 (Hint: Table 15-1)
   ADCSSMUX3_ADC0 = 2; // using AIN2

   // 2.14: Configure ADCSSCTL3 register
   ADCSSCTL3_ADC0 = 0x6; // 0110, enable inturrpt, set last bit
   
   // 2.15: Set the SS3 interrupt mask
   ADCIM_ADC0 |= (1 << 3); // using SS3

   // 2.16: Set the corresponding bit for ADC0 SS3 in NVIC
   ADCISC_ADC0 |= (1 << 3); // clears previous inturrpt
   NVIC_EN0 |= (0x1 << 17); // Enables ADC0 SS3 interrupt. Set field 17 to 1

   // 2.17: Enable ADC0 SS3
   ADCACTSS_ADC0 |= (1 << ASEN3); 

}

void TimerADCTriger_Init(void) {
   // STEP 3: Initialize Timer0A to trigger ADC0 at 1 HZ.
   // Hint: Refer to section 13.3.7 of the datasheet
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
   GPTMTAILR_TIMER_0 = 60000000; // 16000000 

   // step 8, control ADC0 SS3 by timer_0
   GPTMCTL_TIMER_0 |= (1<<5);
   GPTMADCEV_TIMER_0 |= (1 << 0); // trigger ADC when time out

   //step 9
   
   GPTMCTL_TIMER_0 |= (1<<0); // Enables timer 0_A
   }

// NEXT STEP: Go to Lab3_Task1a.c and finish implementing ADC0SS3_Handler
