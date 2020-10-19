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

unsigned long sys_switch_input ( void )
{
  return ( GPIODATA_L & 0x1 ) ; // 0x1 (pressed) or 0 (not pressed)
}

unsigned long ped_switch_input ( void )
{
  return ( GPIODATA_L & 0x2 ) >> 1 ; // 0x1 (pressed) or 0 (not pressed)
}

// turn off all system
void sysOff ( void )
{
  GPIODATA_L = 0x0 ;
}


// turn on greenLED and turn off all other
void onlyGreenOn ( void )
{
    GPIODATA_L |= 0x10;
    GPIODATA_L &= 0x13;
}


void TickTrafficLight() {
    switch (Light_State) { // Transitions
    case TrafficLight_off: // Initial transition
        if (sys_switch_input() == 0x1) {
            while (sys_switch_input() == 0x1){
                Light_State = TrafficLight_go;
            }
        }
        break;
    case TrafficLight_go: 
        if (sys_switch_input() == 0x1) {
            while (sys_switch_input() == 0x1){
                Light_State = TrafficLight_off; 
            }
        }
        break;
    
    default:
        break;
    }

    switch (Light_State) { // Action
    case TrafficLight_off:
        sysOff();
        break;
    case TrafficLight_go:
        onlyGreenOn();
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
