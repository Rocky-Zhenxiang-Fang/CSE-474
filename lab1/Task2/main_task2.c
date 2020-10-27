/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This file is used to simulate traffic light, with all external components connected on Port L
 * */

#include <stdint.h>
#include <stdio.h>
#include "Task_2_header.h"

enum TrafficLightStatus {TrafficLightOff, TrafficLightGo, TrafficLightWarn, TrafficLightStop} LightState; 

int main() {
   SetupRCGCGPIO(); 
   ExternSwitchInit();
   LEDInit();
   LightState = TrafficLightOff;
   while (1) {
      TickTrafficLight();
   }
   return 0;
}


void SetupRCGCGPIO(void) {
   volatile unsigned short delay = 0 ;
   RCGCGPIO |= 0x800 ; // Enable Port L Gating Clock
   delay ++ ;
   delay ++ ;
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

unsigned char SysSwitchPressed (void) {
   if ((GPIODATA_L & 0x1) == 0x1) {
      while ((GPIODATA_L & 0x1) == 0x1) {}  // only proceedes if the button is released
      return 1;
   }
   else {
      return 0; 
   }
}
  
unsigned char PedSwitchPressed (void) {
   if ((GPIODATA_L & 0x2) == 0x2) {
      while ((GPIODATA_L & 0x2) == 0x2) {}  // only proceedes if the button is released
      return 1;
   }
   else {
      return 0; 
   }
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
         LightState = TrafficLightGo;
         } 
      break;

      case TrafficLightGo:
         for (int i = 0; i < 1000000; i += 1) { // waits before turning to red light
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
         if (interupted == 0) {
            LightState = TrafficLightStop; // no interuption, turns to red light
         }
      break; 

      case TrafficLightWarn:
         for (int i = 0; i < 1000000; i += 1) { // waits before turning to red light
            if (SysSwitchPressed() == 1) {
               LightState = TrafficLightOff;
               interupted = 1;
               break;
            }
         }
         if (interupted == 0) {
            LightState = TrafficLightStop; // no interuption, turns to red light
         }
         break;

      case TrafficLightStop:
         for (int i = 0; i < 1000000; i += 1) { // waits before turning to green light
            if (SysSwitchPressed() == 1) {
               LightState = TrafficLightOff;
               interupted = 1;
               break;
            }
         }
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

