#include "header.h"
#include <stdio.h>
#include <stdint.h>

// Initializes UART0 to transfer tempurature data to PC
void UART_Init(void);

// Sends a char c to computer using UART
void Send_Char(char c);

// Send sentance using Send_Char
void Send_sentance(char* sentance); 

int main(int argc, char const *argv[])
{
    UART_Init(); 
    int i = 0;
    while (i != 10) {
        Send_Char(i); 
        i += 1;
    }
    return 0;
}

void UART_Init(void) {

   // 1. Enable the UART module using the RCGCUART register (see page 388).
   RCGCUART |= (1 << 0); // Enable UART0

   // 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 382).
   //    To find out which GPIO port to enable, refer to Table 26-5 on page 1808.
   RCGCGPIO |= (1 << 0); // Enable PortA, PA0 is U0Rx, PA1 is U0Tx

   // 3. Set the GPIO AFSEL bits for the appropriate pins (see page 770). To determine which GPIOs to
   //    configure, see Table 26-4 on page 1797.
   GPIOAFSEL_A = (1 << 1) | (1 << 0); 

   // 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
   //    page 772 and page 780). Not useful for this task

   // 5. Configure the PMCn fields in the GPIOPCTL register to assign
   GPIOPCTL_A = (1 << 0) | (1 << 4); // 4 bits for a pin
   GPIODEN_A = (1 << 0) | (1 << 1); 

   /*
   Baud-Rate = 9600
   Clock is setup to 16 MHZ
   BRD = 16000000 / (16 * 9600) = 104.16666
   UARTFBRD[DIVFRAC] = int(0.16666 * 64 + 0.5) = 11
   */

   // 6. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
   UARTCTL_0 &= ~(1 << 0); 

   // 7. Write the integer portion of the BRD to the UARTIBRD register.
   UARTIBRD_0 = 104; 

   // 8. Write the fractional portion of the BRD to the UARTFBRD register.
   UARTFBRD_0 = 11;

   // 9. Write the desired serial parameters to the UARTLCRH register (in this case, a value of
   //    0x0000.0060).
   UARTLCRH_0 = (0x3 << 5); // 8 bit, no parity, 1-bit stop bits

   // 10. Configure the UART clock source by writing to the UARTCC register.
   UARTCC_0 = 0x0; // Select system clock which will be update by PLL_init()

   // 11. Optionally, configure the μDMA channel (see “Micro Direct Memory Access (μDMA)” on page 678)
   // and enable the DMA option(s) in the UARTDMACTL register.

   // 12. Enable the UART by setting the UARTEN bit in the UARTCTL register.
   UARTCTL_0 = (1 << 0) | (1 << 8) | (1 << 9); // 8 and 9 are for receive and transmition

};

void Send_Char(char data) {
   while ((UARTFR_0 & (1 << 5)) != 0); // wait until finished transmitting previous data
   UARTDR_0 = data; // writes data into UARTDATA
}

void Send_sentance(char* sentance) {
   while (*sentance) {  // while sentance is not pointing at \0
      Send_Char(*sentance);
      sentance += 1; 
   }
}