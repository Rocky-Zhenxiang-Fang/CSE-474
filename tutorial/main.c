#include <stdbool.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

uint32_t i,j; //int 1

int main(void) {
    SYSCTL_RCGCGPIO_R=0X1100; // set clock portn
    i=SYSCTL_RCGCGPIO_R; // delay (more than 3 cycles)
     j=0;
    GPIO_PORTN_DIR_R=0X03;      //enable the GPIO pin for the LED-PN0, set the direction as output, and
    GPIO_PORTN_DEN_R=0X03;  //enable the GPIO pin for digital function
    GPIO_PORTJ_DIR_R=0;
    GPIO_PORTJ_DEN_R=0X03;
    GPIO_PORTJ_PUR_R=0X01;

    while(1){
        GPIO_PORTN_DATA_R = 0X00; //turn led off
/*        while (GPIO_PORTJ_DATA_R & 0X01){
            GPIO_PORTN_DATA_R |=0X01; //turn led on
            for(int j = 0; j < 1000000; j++){}
            GPIO_PORTN_DATA_R &=~0X01; //turn led off again
            for(int j = 0; j < 1000000; j++){}
        }
          for (j=0; i<5; i++)
                {
                GPIO_PORTN_DATA_R |=0X01; //turn led on
                for(int j = 0; j < 1000000; j++){}
                GPIO_PORTN_DATA_R &=~0X01; //turn led off again
                for(int j = 0; j < 1000000; j++){}
                }                     
        GPIO_PORTN_DATA_R |=0X02;  //clear the interrupt flag before return */
    }

}  