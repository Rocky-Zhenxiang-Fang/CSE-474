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
#include "task2_c_header.h"

enum TrafficLightStatus {TrafficLightOff, TrafficLightGo, TrafficLightWarn, TrafficLightStop} LightState; 
unsigned char sysPressed = 0; // Indicates if sys_botton is pressed
unsigned char pedPressed = 0; // Indicates if ped_botton is pressed

int main() {
   SetupGPIO(); 
   SetupTimer();
   LightState = TrafficLightOff;
   while (1) {
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

   switch (LightState) {
      case TrafficLightOff:
         SysOff();
         break;

      case TrafficLightGo:
         GreenOn();
         RedOff();
         YellowOff();
         break; 

      case TrafficLightWarn:
         GreenOff();
         RedOff();
         YellowOn();
         break;

      case TrafficLightStop: 
         GreenOff();
         RedOn();
         YellowOff();
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
      break;

   case TrafficLightWarn:
      LightState = TrafficLightStop; 
      break;

   case TrafficLightStop:
      LightState = TrafficLightGo; 
      break;
      
   default:
      LightState = TrafficLightOff;
      break;
   }
}

void Timer1A_Handler(void) { // system buttom timer
   GPTMICR_TIMER_1 |= (1<<0); // Clear interupt flag
   if (sysPressed) { // the system only responses when the button is held
      if (LightState == TrafficLightOff) {
         LightState = TrafficLightStop; 
         GPTMCTL_TIMER_0 |= (1<<0); // starts timer_0 since the system is on
      }
      else {
         LightState = TrafficLightOff; // Stops the system
         GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
      }
   }
}

void Timer2A_Handler(void) { // ped buttom timer
   GPTMICR_TIMER_2 |= (1<<0); // Clear interupt flag
   if (LightState == TrafficLightGo) {
      LightState = TrafficLightWarn; 
      GPTMTAV_TIMER_0 = 0x00F42400 * 5; // reloads the timer value
   }
}

void PortL_Handler(void) {
   if (((GPIOMIS_L & (1 << sysButton)) >> sysButton) == 0x1) { // sysButton is pressed
      GPIOICR_L |= 0x1 << sysButton; // clear interupt from sysButton
      sysPressed ^= 1; //filp the state of sys button
      if (sysPressed == 1) { // start holding sys button
         GPTMICR_TIMER_1 |= (1<<0); // Clear interupt flag
         GPTMCTL_TIMER_1 |= (1<<0); // Enables timer 1_A, set 1 to field 0
      }
      else {
         GPTMCTL_TIMER_1 &= ~(1<<0); // Disables timer 1_A, set 0 to field 0
      }
   }

   if (((GPIOMIS_L & (1 << pedButton)) >> pedButton) == 0x1) { // pedButton is pressed
      GPIOICR_L |= 0x1 << pedButton; // clear interupt from pedButton
      pedPressed ^= 1; //filp the state of ped button
      if (pedPressed == 1) { // if the button is pressed down
         if (LightState == TrafficLightGo) {// only starts the timer when it is pressed at the Go state
            GPTMICR_TIMER_2 |= (1<<0); // Clear interupt flag
            GPTMCTL_TIMER_2 |= (1<<0); // starts timer_2, ped timer
         }
      }
      else {
         GPTMCTL_TIMER_2 &= ~(1<<0); // Disables timer 2_A, set 0 to field 0
      }
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
   GPTMTAILR_TIMER_0 = 0x00F42400 * 5; 
   GPTMTAILR_TIMER_1 = 0x00F42400 * 2; 
   GPTMTAILR_TIMER_2 = 0x00F42400 * 2; 

   //step 8, timer inturrupt
   GPTMIMR_TIMER_0 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   GPTMIMR_TIMER_1 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   GPTMIMR_TIMER_2 |= (0x1<<0); // Set interupt mode to time out. Set field 0 to 1
   NVIC_EN0 |= (0x1<<19); // Enables timer_0 interrupt. Set field 19 to 1
   NVIC_EN0 |= (0x1<<21); // Enables timer_1 interrupt. Set field 21 to 1
   NVIC_EN0 |= (0x1<<23); // Enables timer_2 interrupt. Set field 23 to 1
   //step 9
}

void GreenOn(void) {
   GPIODATA_L |= 0x10;
}

void GreenOff(void) {
   GPIODATA_L &= ~0x10;
}

void RedOn(void) {
   GPIODATA_L |= 0x04;
}

void RedOff(void) {
   GPIODATA_L &= ~0x04;
}

void YellowOn(void) {
   GPIODATA_L |= 0x08;
}

void YellowOff(void) {
   GPIODATA_L &= ~0x08;
}

void SysOff (void) {
   RedOff();
   GreenOff();
   YellowOff();
}
