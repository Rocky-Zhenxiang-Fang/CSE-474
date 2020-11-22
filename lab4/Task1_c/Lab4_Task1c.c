/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/17
 * This file is used print the temperature of the tiva board to the debugger terminal
 */

#include <stdint.h>
#include <stdio.h>
#include "Lab4_Inits_1c.h"
#include "SSD2119_Display.h"
#include "task1_c_header.h"

uint32_t ADC_value = 0;
unsigned char printValue = 0; 
char temperatureString[50];   // The string to be printed at the first line containing the temeperature
char clockString[100];        // The string to be printed at the second line containing the clock frequency
unsigned long LastXPos = 0;       // Records the x coordinate of the last touch of the LCD
unsigned long LastYPos = 0;       // Records the y coordinate of the last touch of the LCD

unsigned char usingFreq = PRESET2;

// Draws two botton on the LCD panel, one for 12MHz, the other from 120MHz
void LCD_DrawButtons(void); 

// takes two strings and present it at the top of the LCD panel
void LCD_PrintTempearture(char temperatureString[]);  

// prints clock time to the LCD panel
void LCD_PrintClock(char clockFrequencyString[]);

// FSM machine tick function
void TickFreq(void); 


int main(void) {
   // set up functions
   enum frequency freq = PRESET2; // 60 MHz
   PLL_Init(freq);            // Set system clock frequency to 60 MHz
   Switch_Init();             // Initialize the 2 onboard Switches (GPIO)
   TimerADCTriger_Init();     // Initialize timer_0 that triggers the ADC
   ADCReadPot_Init();         // Initialize ADC0 to read from the potentiometer
   LCD_Init();                // Initialize LCD panel
   LCD_DrawButtons();         // Draws two button on LCD panel
   snprintf(clockString, 50, "The current clock frequency is 60 MHz. \n");

   while(1) {
      TickFreq(); 
   }
   return 0;
}

void TickFreq(void) {
   // transition
   switch (usingFreq) {
   case 60:
      // Nothing right now
      break;
   case 12:
      // Nothing right now
   case 120: 
      // Nothing right now
   default:
      usingFreq = 60 ;
      break;
   }

   // Action
   // All states do the same thing, converting the temperature and print it on the LCD panel
   if (printValue) { // only print if ADC value has been changed
      float tempurature_c;
      float tempurature_f; 
      tempurature_c = 147.5 - (75 * 3.3 * ADC_value / 4096);
      tempurature_f = tempurature_c * 1.8 + 32; 
      sprintf(temperatureString, "The current temperature is %lf C, %lf F. \n", tempurature_c, tempurature_f); 
      LCD_PrintTempearture(temperatureString);
      LCD_PrintClock(clockString);
      printValue ^= 1;
   }

}

void LCD_DrawButtons(void) {
   // Draw button for 12 MHz
   LCD_DrawFilledRect(10, 120, 140, 100, Color4[5]);
   LCD_SetCursor(60, 170);
   LCD_PrintString("12 MHz"); 

   // Draw button for 120 MHz
   LCD_DrawFilledRect(170, 120, 140, 100, Color4[3]);
   LCD_SetCursor(220, 170);
   LCD_PrintString("120 MHz"); 
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
      usingFreq = 12; 
      LCD_PrintClock(clockString);
   }

   if (((GPIOMIS_J & (1 << SW2)) >> SW2) == 0x1) { // Sw2 is pressed, 120MHz colck
      GPIOICR_J |= 0x1 << SW2; // clear interupt from SW2
      PLL_Init(PRESET1); 
      GPTMTAILR_TIMER_0 = 120000000; 
      sprintf(clockString, "The current clock frequency is 120 MHz. \n");
      usingFreq = 120; 
      LCD_PrintClock(clockString);
   }
}
