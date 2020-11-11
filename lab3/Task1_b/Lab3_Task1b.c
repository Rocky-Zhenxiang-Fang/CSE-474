/**
 * EE/CSE 474: Lab3 Task1a main function
 */

// NOTE: This is the main function for Task 1a. You should start by
// implementing Lab3_Driver.c (STEP 0a, 1-3) and then come back to finish
// the ISR and the main function (STEP 0b, 4-5).

#include <stdint.h>
#include <stdio.h>
#include "Lab3_Inits_b.h"

// STEP 0b: Include your header file here
// YOUR CUSTOM HEADER FILE HERE
#include "task1_b_header.h"

uint32_t ADC_value;

int main(void) {
   // Select system clock frequency preset
   enum frequency freq = PRESET2; // 60 MHz
   PLL_Init(freq);            // Set system clock frequency to 60 MHz
   Switch_Init();             // Initialize the 2 onboard Switches (GPIO)
   ADCReadPot_Init();         // Initialize ADC0 to read from the potentiometer
   float tempurature;
   while(1) {
      tempurature = (ADC_value / 4095.0) * 10.0;
      printf("tempurature = %f \n", tempurature); 
   }
   return 0;
}

void ADC0SS3_Handler(void) {
   // STEP 4: Implement the ADC ISR.
   // 4.1: Clear the ADC0 interrupt flag
   ADCISC_ADC0 |= (1 << 3); // clears previous inturrpt

   // 4.2: Save the ADC value to global variable ADC_value
   ADC_value = ADCSSFIFO3_ADC0; 
}

void PortJ_Handler(void) {
   if (((GPIOMIS_J & (1 << SW1)) >> SW1) == 0x1) { // Sw1 is pressed, 12MHZ colck
      GPIOICR_J |= 0x1 << SW1; // clear interupt from SW1
      PLL_Init(PRESET3); 
   }

   if (((GPIOMIS_J & (1 << SW2)) >> SW2) == 0x1) { // Sw2 is pressed, 120MHz colck
      GPIOICR_J |= 0x1 << SW2; // clear interupt from SW2
      PLL_Init(PRESET1); 
   }
}
