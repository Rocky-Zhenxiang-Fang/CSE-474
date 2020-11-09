/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/28
 * This file is used to simulate traffic light, with all external components connected on Port L
 * Timer_0 is used periodicly to cacultate time period at 0.2Hz.
 * Timer_1 is used in one shoot mode with time set 2sec.  
 * Port_L connects all external button and LEDs
 * */

#include <stdint.h>
#include <stdio.h>
#include "task1_b_header.h"

enum TrafficLightStatus {TrafficLightOff, TrafficLightGo, TrafficLightWarn, TrafficLightStop} LightState; 

int main() {
   SetupGPIO(); 
   SetupTimer();
   LightState = TrafficLightOff;
   while (1) {
      TickTrafficLight();
   }
   return 0;
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
   RCGCTIMER |= (1<<1); // Enables GPtimer 0, sets 1 at field 1

   //step 2
   GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
   GPTMCTL_TIMER_0 &= ~(1<<8); //Disables timer 0_B, set 0 to field 8
   GPTMCTL_TIMER_1 &= ~(1<<0); //Disables timer 1_A, set 0 to field 0
   GPTMCTL_TIMER_1 &= ~(1<<8); //Disables timer 1_B, set 0 to field 8

   //step 3
   GPTMCFG_TIMER_0 = 0x00000000; 
   GPTMCFG_TIMER_1 = 0x00000000; 

   //step 4
   GPTMCFG_TIMER_0 = 0x0; // select 32-bit mode
   GPTMCFG_TIMER_1 = 0x0; // select 32-bit mode

   //step 5
   GPTMTAMR_TIMER_0 = (0x1<<0); // select the one shoot mode for timer_0
   GPTMTAMR_TIMER_1 = (0x1<<0); // select the one shoot mode for timer_1

   //step 6
   GPTMTAMR_TIMER_0 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4
   GPTMTAMR_TIMER_1 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4

   //step 7
   GPTMTAILR_TIMER_0 = 0x00F42400 * 5; // 16000000 * 5 = 5 secs
   GPTMTAILR_TIMER_1 = 0x00F42400 * 2; // 16000000 * 2 = 2 secs

   //step 9
}

unsigned char SysSwitchPressed (void) {
   unsigned char valid = 0; 
   if ((GPIODATA_L & 0x1) == 0x1) {
      GPTMCTL_TIMER_1 |= (1<<0); //Starts timer_1 countdown
      while ((GPIODATA_L & 0x1) == 0x1) { // as the button is held
         if ((GPTMRIS_TIMER_1 & TATORIS) == 1) { // finished countdown
            valid = 1;  // onlt this condition is valid
         }
      }  
   }
   GPTMICR_TIMER_1 |= (1<<0);  // reset flag
   GPTMCTL_TIMER_1 &= ~(1<<0); //Disables timer 1_A, set 0 to field 0
   return valid; 
}
  
unsigned char PedSwitchPressed (void) {
   unsigned char valid = 0;
   if ((GPIODATA_L & 0x2) == 0x2) {
      GPTMCTL_TIMER_1 |= (1<<0); //Starts timer_1 countdown
      while ((GPIODATA_L & 0x2) == 0x2) { // as the button is held
         if ((GPTMRIS_TIMER_1 & TATORIS) == 1) { // finished countdown
            valid = 1;  // onlt this condition is valid
         }
      }
   }
   GPTMICR_TIMER_1 |= (1<<0);  // reset flag
   GPTMCTL_TIMER_1 &= ~(1<<0); //Disables timer 1_A, set 0 to field 0
   return valid; 
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

void TickTrafficLight(void) {
   unsigned char interupted = 0;
   switch (LightState) { // transition
      case TrafficLightOff:
         if (SysSwitchPressed() == 1) { // turn on the system
         LightState = TrafficLightStop;
         } 
         break;

      case TrafficLightGo:
         GPTMCTL_TIMER_0 |= (1<<0); // Enables timer_0
         while ((GPTMRIS_TIMER_0 & TATORIS) != 1) {
            if (SysSwitchPressed() == 1) {
               LightState = TrafficLightOff;
               interupted = 1;
               break;
            }
            if (PedSwitchPressed() == 1){ // pedstrin shows up
               LightState = TrafficLightWarn; 
               interupted = 1;
               break;
            }
         }
         GPTMICR_TIMER_0 |= (1<<0); // reset flag
         GPTMCTL_TIMER_0 &= ~(1<<0); // Disables timer 0_A, set 0 to field 0
         GPTMTAILR_TIMER_0 = 0x00F42400 * 5; // 16000000 * 5 = 5 secs, reset the time
         if (interupted == 0) {
            LightState = TrafficLightStop; // no interuption, turns to red light
         } 
         break; 

      case TrafficLightWarn:
         GPTMCTL_TIMER_0 |= (1<<0); // Enables timer_0
         while ((GPTMRIS_TIMER_0 & TATORIS) != 1) { // waits before turning to red light
            if (SysSwitchPressed() == 1) {
               LightState = TrafficLightOff;
               interupted = 1;
               break;
            }
         }
         GPTMICR_TIMER_0 |= (1<<0); // reset flag
         GPTMCTL_TIMER_0 &= ~(1<<0); // Disables timer 0_A, set 0 to field 0
         GPTMTAILR_TIMER_0 = 0x00F42400 * 5; // 16000000 * 5 = 5 secs
         if (interupted == 0) {
            LightState = TrafficLightStop; // no interuption, turns to red light
         }

         break;

      case TrafficLightStop:
         GPTMCTL_TIMER_0 |= (1<<0); // Enables timer_0
         while ((GPTMRIS_TIMER_0 & TATORIS) != 1) { // waits before turning to green light
            if (SysSwitchPressed() == 1) {
               LightState = TrafficLightOff;
               interupted = 1;
               break;
            }
         }
         GPTMICR_TIMER_0 |= (1<<0); // reset flag
         GPTMCTL_TIMER_0 &= ~(1<<0); // Disables timer 0_A, set 0 to field 0
         GPTMTAILR_TIMER_0 = 0x00F42400 * 5; // 16000000 * 5 = 5 secs
         if (interupted == 0) {
            LightState = TrafficLightGo; // no interuption, turns to green light
         } 
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

