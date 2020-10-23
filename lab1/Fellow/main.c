#include "lab1.h"
#include <stdint.h>
#include <stdbool.h>
//#include "RIMS.h"

bool x = false;


void LED_init ( void ){
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= 0x1004 ; // activate clock for Port C
  delay ++ ;
  delay ++ ;
  GPIOAMSEL_C &= ~ 0x70 ; // disable analog function of PC4
  GPIODIR_C |= 0x70 ; // set PC4 to output
  GPIOAFSEL_C &= ~ 0x70 ; // set PC4 regular port function
  GPIODEN_C |= 0x70 ; // enable digital output on PC4
}
// turn on LED connected to PC4
void turn_on_all(void){ 
  GPIODEN_C |= 0x70 ;
}

void LED_on_pc4 ( void )
{
  GPIODATA_C |= 0x10 ;
}
// turn off LED connected to PC4
void LED_off_pc4 ( void )
{
  GPIODATA_C &= ~ 0x10 ;
}
void LED_on_pc5 ( void )
{
  GPIODATA_C |= 0x20 ;
}
// turn off LED connected to PC4
void LED_off_pc5 ( void )
{
  GPIODATA_C &= ~ 0x20 ;
}
void LED_on_pc6 ( void )
{
  GPIODATA_C |= 0x40 ;
}
// turn off LED connected to PC4
void LED_off_pc6 ( void )
{
  GPIODATA_C &= ~ 0x40 ;
}
void extern_switch_init ( void )
{
volatile unsigned short delay = 0 ;
//RCGCGPIO |= 0x1000 ; // Enable Port N Gating Clock
delay ++ ;
delay ++ ;
GPIOAMSEL_N &= ~ 0x0c ; // Disable PN2 analog function
GPIOAFSEL_N &= ~ 0x0c ; // Select PN2 regular port function
GPIODIR_N &= ~ 0x0c ; // Set PN2 to input direction
GPIODEN_N |= 0x0c ; // Enable PN2 digital function
}
unsigned long switch_input_A ( void )
{
return ( GPIODATA_N & 0x04 ) ; // 0x4 (pressed) or 0 (not pressed)
}
unsigned long switch_input_B ( void )
{
return ( GPIODATA_N & 0x08 ) ; // 0x4 (pressed) or 0 (not pressed)
}
void turn_off_all( void ){
  LED_off_pc4();
  LED_off_pc5();
  LED_off_pc6();
}



enum LA_States { LA_SMStart, go, stop, warn , LA_end } LA_State;
void TickFct_Latch()
{
  switch(LA_State) {   // Transitions
     case LA_SMStart:  // Initial transition
        LA_State = go;
        break;

     case go:
       LA_State = stop;
       LED_off_pc5();
       LED_off_pc6();
       LED_on_pc4();
       for(int i = 0 ; i <1000000; i++){
        if(switch_input_B()){
           LA_State = warn;           
         }
        if(switch_input_A()){          
           turn_off_all();
           LA_State = LA_end;
           x = true;  
         }    
       }
        break;

     case warn:
        LA_State = stop;
        break;
        
     case stop:
         LA_State = go;
         break;
     case LA_end:
         break; 
        
     default:
        LA_State = LA_SMStart;
        break;
  } // Transitions

  switch(LA_State) {   // State actions
     case go:
        break;

     case warn:
       LED_off_pc4();
       LED_off_pc6();
       LED_on_pc5();
       for(int i = 0 ; i <1000000; i++){
       
       if(switch_input_A()){
           
           turn_off_all();
           LA_State = LA_end;
           x = true;
           
         }
       
       }
        break;
     case stop:
       LED_off_pc4();
       LED_off_pc5();
       LED_on_pc6();
       for(int i = 0 ; i <1000000; i++){
       
         if(switch_input_A()){
           
           turn_off_all();
           LA_State = LA_end;
           x = true;
         }
       
       }
       break;
       
       case LA_end:
         break; 
      
       
     default:
        break;
   } // State actions
}

 int main(){
  volatile unsigned short delay = 0 ;
  LED_init();
  extern_switch_init();
  //LA_State = LA_SMStart;
  while(1) {
    if(switch_input_A()){
      for(int i = 0 ; i <500000; i++)
      LA_State = LA_SMStart;
      while(x == false){
        TickFct_Latch();
        }
       x = false;
    }
  }
 
  
  
  return 0;
}
