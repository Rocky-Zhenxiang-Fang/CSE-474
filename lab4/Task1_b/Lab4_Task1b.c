/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/17
 * This file is used print the temperature of the tiva board to the debugger terminal
 */
#include <stdint.h>
#include <stdio.h>
#include "Lab4_Inits_b.h"
#include "SSD2119_Display.h"
#include "task1_b_header.h"

uint32_t ADC_value = 0;
unsigned char printValue = 0; 
char temperatureString[50]; 
char clockString[100]; 

// takes two strings and present it at the top of the LCD panel
void LCD_PrintTempearture(char temperatureString[]);  

// prints clock time to the LCD panel
void LCD_PrintClock(char clockFrequencyString[]);


int main(void) {
   // Select system clock frequency preset
   enum frequency freq = PRESET2; // 60 MHz
   PLL_Init(freq);            // Set system clock frequency to 60 MHz
   Switch_Init();             // Initialize the 2 onboard Switches (GPIO)
   TimerADCTriger_Init();     // Initialize timer_0 that triggers the ADC
   ADCReadPot_Init();         // Initialize ADC0 to read from the potentiometer
   LCD_Init();                // Initialize LCD panel

   float tempurature_c;
   long touchedX; 
   long touchedY; 
   snprintf(clockString, 50, "The current clock frequency is 60 MHz. \n");
   while(1) {

      if (printValue) { // only print if ADC value has been changed
         float tempurature_f; 
         tempurature_c = 147.5 - (75 * 3.3 * ADC_value / 4096);
         tempurature_f = tempurature_c * 1.8 + 32; 
         sprintf(temperatureString, "The current temperature is %lf C, %lf F. \n", tempurature_c, tempurature_f); 
         LCD_PrintTempearture(temperatureString);
         LCD_PrintClock(clockString);
         printValue ^= 1;
      }
   }
   return 0;
}


void LCD_PrintTempearture(char temperatureString[]) {
   LCD_SetCursor(0, 0); 
   LCD_PrintString(temperatureString); 
} 

void LCD_PrintClock(char clockFrequencyString[]) {
   LCD_SetCursor(0, 10);
   LCD_PrintString(clockFrequencyString);
}

void ADC0SS3_Handler(void) {
   ADCISC_ADC0 |= (1 << 3); // clears previous inturrpt
   ADC_value = ADCSSFIFO3_ADC0; // Save the ADC value to global variable ADC_value
   printValue ^= 1; // indicates new value should be printed
}

void PortJ_Handler(void) {
   if (((GPIOMIS_J & (1 << SW1)) >> SW1) == 0x1) { // Sw1 is pressed, 12MHZ colck
      GPIOICR_J |= 0x1 << SW1; // clear interupt from SW1
      PLL_Init(PRESET3); 
      GPTMTAILR_TIMER_0 = 12000000; 
      sprintf(clockString, "The current clock frequency is 12 MHz. \n");
      LCD_PrintClock(clockString);
   }

   if (((GPIOMIS_J & (1 << SW2)) >> SW2) == 0x1) { // Sw2 is pressed, 120MHz colck
      GPIOICR_J |= 0x1 << SW2; // clear interupt from SW2
      PLL_Init(PRESET1); 
      GPTMTAILR_TIMER_0 = 120000000; 
      sprintf(clockString, "The current clock frequency is 120 MHz. \n");
      LCD_PrintClock(clockString);
   }
}