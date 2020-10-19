#include <stdint.h>
#include "testheader.h"

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
  return ( GPIODATA_L & 0x2 ) ; // 0x2 (pressed) or 0 (not pressed)
}


// turn on LED connected to PC4
void LED_on ( void )
{
  GPIODATA_L |= 0x1C ;
}

// turn off LED connected to PC4
void LED_off ( void )
{
  GPIODATA_L &= ~ 0x1C ;
}

int main()
{
    setup_RCGCGPIO(); 
    extern_switch_init();
    LED_init();
    
  while (1) {
    if (sys_switch_input() == 0x1) {
        LED_on();
    }
    if (ped_switch_input() == 0x2) {
        LED_off();
    }
  }
  return 0;
}
