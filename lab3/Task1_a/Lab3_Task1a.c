/**
 * EE/CSE 474: Lab3 Task1a main function
 * 
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/17
 * This file is used show the resistance for a external potentiometer using onboard LEDs
 */

#include <stdint.h>
#include <stdio.h>
#include "Lab3_Inits_1a.h"
#include "task1_a_header.h"

uint32_t ADC_value;

int main(void) {
   // Select system clock frequency preset
   enum frequency freq = PRESET2; // 60 MHz
   PLL_Init(freq);        // Set system clock frequency to 60 MHz
   LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
   ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
   TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
   float resistance;
   while(1) {
      // Convert ADC_value to resistance in kilo-ohm
      resistance = (ADC_value / 4095.0) * 10.0;
      printf("resistance = %f \n", resistance); 
      
      // Change the pattern of LEDs based on the resistance
      if (resistance < 2.5) {
         GPIODATA_N = LED1;
         GPIODATA_F = 0x0;
      } else if (resistance < 5.0) {
         GPIODATA_N = LED1 + LED2;
         GPIODATA_F = 0x0;
      } else if (resistance < 7.5) {
         GPIODATA_N = LED1 + LED2;
         GPIODATA_F = LED3; 
      } else {
         GPIODATA_N = LED1 + LED2;
         GPIODATA_F = LED3 + LED4;         
      }
   }
   return 0;
}

void ADC0SS3_Handler(void) {
   ADCISC_ADC0 |= (1 << 3); // clears previous inturrpt
   ADC_value = ADCSSFIFO3_ADC0; // Save the ADC value to global variable ADC_value
}
