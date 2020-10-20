#include <stdint.h>
#include "lab1.h"

enum TrafficLightStatus {TrafficLight_off, TrafficLight_go, TrafficLight_warn, TrafficLight_stop} Light_State; 

void setup_RCGCGPIO(void) {
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= 0x400 ; // Enable Port L Gating Clock
  delay ++ ;
  delay ++ ;
}

void extern_switch_init ( void )
{
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= 0x800 ; // Enable Port L Gating Clock
  delay ++ ;
  delay ++ ;
  GPIOAMSEL_L &= ~ 0x3 ; // Disable PL0, PL1 analog function
  GPIOAFSEL_L &= ~ 0x3 ; // Select PL0, PL1 regular port function
  GPIODIR_L &= ~ 0x3 ; // Set PL0, PL1 to input direction
  GPIODEN_L |= 0x3 ; // Enable PL0, PL1 digital function
}

void LED_init ( void )
{
  GPIOAMSEL_L &= ~ 0x1C ; // disable analog function of PL2, PL3, PL4
  GPIOAFSEL_L &= ~ 0x1C ; // set PL2, PL3, PL4 regular port function
  GPIODIR_L |= 0x1C ; // set PL2, PL3, PL4 to output
  GPIODEN_L |= 0x1C ; // enable digital output on PL2, PL3, PL4
  GPIODATA_L &= ~ 0x1C ;
}

unsigned char sys_switch_pressed ( void ) {
  if (GPIODATA_L & 0x1 == 0x1) {
    while (GPIODATA_L & 0x1 == 0x1) {}  // only proceedes if the button is released
    return 1;
  }
  else
  {
     return 0; 
  }
}
  
 unsigned char ped_switch_pressed ( void ) {
  if (GPIODATA_L & 0x2 == 0x2) {
    while (GPIODATA_L & 0x2 == 0x2) {}  // only proceedes if the button is released
    return 1;
  }
  else
  {
     return 0; 
  }
}


void greenOn(void) {
  GPIODATA_L |= 0x10;
}

void greenOff(void) {
  GPIODATA_L &= ~0x10;
}

void redOn(void) {
  GPIODATA_L |= 0x04;
}

void redOff(void) {
  GPIODATA_L &= ~0x04;
}

void yellowOn(void) {
  GPIODATA_L |= 0x08;
}

void yellowOff(void) {
  GPIODATA_L &= ~0x08;
}

// turn off all system
void sysOff ( void )
{
  redOff();
  greenOff();
  yellowOff();
}


void TickTrafficLight() {
  switch (Light_State) 
  {
    case TrafficLight_off:
      if (sys_switch_pressed() == 1) {
        Light_State = TrafficLight_go;
      }
      break;
    case TrafficLight_go:
      if (sys_switch_pressed() == 1) {
        Light_State = TrafficLight_off;
      }
    
    default:
      break;
  }

  switch (Light_State)
  {
    case TrafficLight_off:
      sysOff();
      break;
    case TrafficLight_go:
      greenOn();
      redOn();
      yellowOn();
      break;

  default:
    break;
  } 
}
int main()
{
    setup_RCGCGPIO(); 
    extern_switch_init();
    LED_init();
    Light_State = TrafficLight_off;
  while (1) {
      TickTrafficLight();
    }
  
  return 0;
}
