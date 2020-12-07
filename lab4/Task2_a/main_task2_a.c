/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/28
 * This file is used to simulate traffic light, with all external components connected on Port L
 * Timer_0 is used in periodic mode with time set 5 sec. 
 * Timer_1 is used in one shoot mode with time set 2 sec for sys_button.  
 * Timer_2 is used in one shoot mode with time set 2 sec for ped_button. 
 * Port_L connects all external button and LEDs, sys_button = PL0, ped_button = PL1, red = PL2, yellow = PL3, green = PL4
 * */

#include <stdint.h>
#include <stdio.h>
#include "task2_a_header.h"
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "PLL_Header.h" 

enum TrafficLightStatus {TrafficLightOff, TrafficLightGo, TrafficLightWarn, TrafficLightStop} LightState; 
unsigned char sysPressed = 0; // Indicates if sys_botton is pressed
unsigned char pedPressed = 0; // Indicates if ped_botton is pressed
unsigned char stateChanged = 0; // Indicates that a state has been changed, so the LCD needs to turn black
unsigned short LastXPos = 0;   // Indicates the x coordinate of the last touch of the LCD
unsigned short LastYPos = 0; // Indicates the y coordinate of the last touch of the LCD

int main() {
   PLL_Init(PRESET2);   // Using 60Hz clock
   LCD_Init();                // Initialize LCD panel
   Touch_Init();              // Initialize touch function of LCD
   DrawUI(); 
   SetupGPIO(); 
   SetupTimer();
   LightState = TrafficLightOff;
   while (1) {
      ReadButton(); 
      TickTrafficLight();
   }
   return 0;
}

void TickTrafficLight(void) {
   switch (LightState) { // transition
      case TrafficLightOff:
         // this transition is all controled by interruption
         break;

      case TrafficLightGo:
         // this transition is all controled by interruption
         break; 

      case TrafficLightWarn:
         // this transition is all controled by interruption
         break;

      case TrafficLightStop:
         // this transition is all controled by interruption
         break;

      default:
         LightState = TrafficLightOff; 
         break;
  }

   if (stateChanged == 1) {
      stateChanged = 0; 
      LCD_DrawFilledRect(0, 80, 320, 150, Color4[0]);
   }

   switch (LightState) {
      case TrafficLightOff:
         SysOff();
         break;

      case TrafficLightGo:
         GreenOn();
         break; 

      case TrafficLightWarn:
         YellowOn();
         break;

      case TrafficLightStop: 
         RedOn();
         break;
         
      default:
         SysOff();
         break;
   }
}

void Timer0A_Handler(void) { // 5 sec timer
   GPTMICR_TIMER_0 |= (1<<0); // Clear interupt flag
   switch (LightState) {
      case TrafficLightGo:
         LightState = TrafficLightStop;
         stateChanged = 1; 
         break;

      case TrafficLightWarn:
         LightState = TrafficLightStop;
         stateChanged = 1;
         break;

      case TrafficLightStop:
         LightState = TrafficLightGo; 
         stateChanged = 1;
         break;
         
      default:
         LightState = TrafficLightOff;
         stateChanged = 1;
         break;
   }
}

void Timer1A_Handler(void) { // system buttom timer
   GPTMICR_TIMER_1 |= (1<<0); // Clear interupt flag
   if (sysPressed) { // the system only responses when the button is held
      if (LightState == TrafficLightOff) {
         LightState = TrafficLightStop; 
         stateChanged = 1;
         GPTMCTL_TIMER_0 |= (1<<0); // starts timer_0 since the system is on
      }
      else {
         LightState = TrafficLightOff; // Stops the system
         stateChanged = 1;
         GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
         GPTMTAV_TIMER_0 = 60000000 * 5; // reloads the timer value
      }
   }
}

void Timer2A_Handler(void) { // ped buttom timer
   GPTMICR_TIMER_2 |= (1<<0); // Clear interupt flag
   if (LightState == TrafficLightGo) {
      LightState = TrafficLightWarn;
      stateChanged = 1;
      GPTMTAV_TIMER_0 = 60000000 * 5; // reloads the timer value
   }
}

void DrawUI(void) {
   LCD_DrawRect(10, 10, 140, 60, Color4[11]); 
   LCD_DrawRect(170, 10, 140, 60, Color4[13]); 
   LCD_SetCursor(40, 35); 
   LCD_PrintString("System button"); 
   LCD_SetCursor(190, 35); 
   LCD_PrintString("Pedestrian button");
}

void ReadButton(void) {
   LastXPos = Touch_ReadX(); 
   LastYPos = Touch_ReadY();
   if ((600 <= LastXPos && LastXPos <= 1450) && (1430 <= LastYPos && LastYPos <= 1650)) {
      sysPressed = 1;
      pedPressed = 0; 
   } 
   else if ((1460 <= LastXPos && LastXPos <= 2000) && (1430 <= LastYPos && LastYPos <= 1650)) {
      sysPressed = 0;
      pedPressed = 1; 
   }
   else {
      sysPressed = 0;
      pedPressed = 0; 
   }

   if (sysPressed == 1 && pedPressed == 0) { // Action of the system button
      if ((GPTMCTL_TIMER_1 & 0x1) != 1) { // The system button timer is disabled
         GPTMICR_TIMER_1 |= (1<<0); // Clear interupt flag
         GPTMTAILR_TIMER_1 = 60000000 * 2;  // Reloads value
         GPTMCTL_TIMER_1 |= (1<<0); // Enables timer 1_A, set 1 to field 0
      }
   }
   else if (sysPressed == 0 && pedPressed == 1){ // Action of the pedstrain button
      if ((GPTMCTL_TIMER_2 & 0x1) != 1) { // The pedstrain button timer is disabled
         GPTMICR_TIMER_2 |= (1<<0); // Clear interupt flag
         GPTMTAILR_TIMER_2 = 60000000 * 2;  // Reloads value
         GPTMCTL_TIMER_2 |= (1<<0); // Enables timer 2_A, set 1 to field 0
      }
   }
   else { // No button is held
      // Disables button timers
      GPTMCTL_TIMER_1 &= ~(1<<0); //Disables timer 1_A, set 0 to field 0
      GPTMCTL_TIMER_2 &= ~(1<<0); //Disables timer 2_A, set 0 to field 0
      // reloads timer value
      GPTMTAILR_TIMER_1 = 60000000 * 2; 
      GPTMTAILR_TIMER_2 = 60000000 * 2; 
   }
}

void SetupGPIO(void) {
   volatile unsigned short delay = 0 ;
   RCGCGPIO |= (1<<10) ; // Enable Port L Gating Clock, set 1 for field 10
   delay ++ ;
   delay ++ ;
   ExternSwitchInit();
   LEDInit();
}

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

void ExternSwitchInit (void) {
   GPIOAMSEL_L &= ~ 0x3 ; // Disable PL0, PL1 analog function
   GPIOAFSEL_L &= ~ 0x3 ; // Select PL0, PL1 regular port function
   GPIODIR_L &= ~ 0x3 ; // Set PL0, PL1 to input direction
   GPIODEN_L |= 0x3 ; // Enable PL0, PL1 digital function

   // Prot_L (Switches) Interrupt
   GPIOIS_L &= ~0x3; // make switches edge sensitive
   GPIOIBE_L |= 0x3; // detect both edges
   GPIOICR_L |= 0x3; // clear prior interrupt
   GPIOIM_L |= 0x3; // unmask interrupt
   NVIC_EN1 |= (0x1 << 21); // Enables Port L interrupt. Set field 21 to 1
}

void LEDInit (void) {
   GPIOAMSEL_L &= ~ 0x1C ; // disable analog function of PL2, PL3, PL4
   GPIOAFSEL_L &= ~ 0x1C ; // set PL2, PL3, PL4 regular port function
   GPIODIR_L |= 0x1C ; // set PL2, PL3, PL4 to output
   GPIODEN_L |= 0x1C ; // enable digital output on PL2, PL3, PL4
   GPIODATA_L &= ~ 0x1C ;
}

void SetupTimer(void) {
   //step 1
   RCGCTIMER |= (1<<0); // Enables GPtimer 0, sets 1 at field 0
   RCGCTIMER |= (1<<1); // Enables GPtimer 1, sets 1 at field 1
   RCGCTIMER |= (1<<2); // Enables GPtimer 2, sets 1 at field 2

   //step 2
   GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
   GPTMCTL_TIMER_0 &= ~(1<<8); //Disables timer 0_B, set 0 to field 8
   GPTMCTL_TIMER_1 &= ~(1<<0); //Disables timer 1_A, set 0 to field 0
   GPTMCTL_TIMER_1 &= ~(1<<8); //Disables timer 1_B, set 0 to field 8
   GPTMCTL_TIMER_2 &= ~(1<<0); //Disables timer 2_A, set 0 to field 0
   GPTMCTL_TIMER_2 &= ~(1<<8); //Disables timer 2_B, set 0 to field 8

   //step 3
   GPTMCFG_TIMER_0 = 0x00000000; 
   GPTMCFG_TIMER_1 = 0x00000000; 
   GPTMCFG_TIMER_2 = 0x00000000; 

   //step 4
   GPTMCFG_TIMER_0 = 0x0; // select 32-bit mode
   GPTMCFG_TIMER_1 = 0x0; // select 32-bit mode
   GPTMCFG_TIMER_2 = 0x0; // select 32-bit mode

   //step 5
   GPTMTAMR_TIMER_0 = (0x2<<0); // select the periodic mode for timer_0
   GPTMTAMR_TIMER_1 = (0x1<<0); // select the one shoot mode for timer_1
   GPTMTAMR_TIMER_2 = (0x1<<0); // select the one shoot mode for timer_2

   //step 6
   GPTMTAMR_TIMER_0 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4
   GPTMTAMR_TIMER_1 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4
   GPTMTAMR_TIMER_2 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4
   //step 7
   GPTMTAILR_TIMER_0 = 60000000 * 5; 
   GPTMTAILR_TIMER_1 = 60000000 * 2; 
   GPTMTAILR_TIMER_2 = 60000000 * 2; 

   //step 8, timer inturrupt
   GPTMIMR_TIMER_0 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   GPTMIMR_TIMER_1 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   GPTMIMR_TIMER_2 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1

   NVIC_EN0 |= (0x1<<19); // Enables timer_0 interrupt. Set field 19 to 1
   NVIC_EN0 |= (0x1<<21); // Enables timer_1 interrupt. Set field 21 to 1
   NVIC_EN0 |= (0x1<<23); // Enables timer_2 interrupt. Set field 23 to 1
}

void GreenOn(void) {
   GPIODATA_L |= 0x10;
   LCD_DrawFilledCircle(64, 120, 32, Color4[10]);
}

void RedOn(void) {
   GPIODATA_L |= 0x04;
   LCD_DrawFilledCircle(256, 120, 32, Color4[12]); 
}

void YellowOn(void) {
   GPIODATA_L |= 0x08;
   LCD_DrawFilledCircle(160, 120, 32, Color4[14]);
}

void SysOff (void) {
   LCD_DrawFilledRect(0, 80, 320, 150, Color4[0]);
}
