/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/17
 * This file is used to send the character received from UART back to where it is from
 */

#include <stdint.h>
#include <stdio.h>
#include "Lab3_task2_b_header.h"


int main(void) {
   enum frequency freq = PRESET2; // 60 MHz
   PLL_Init(freq);            // Set system clock frequency to 60 MHz
   UART_Init(); // Initialize UART
   while(1) {
      Send_sentance("Please press a character. \n \r");
      char received_data = Read_Char();
      char displaying_text[100];
      sprintf(displaying_text, "Character pressed is: %c \n \r", received_data); 
      Send_sentance(displaying_text); 
      Send_sentance("\n \r"); // for aesthetic  reason
   }
   return 0;
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

void UART_Init(void) {
   RCGCUART |= (1 << 3); // Enable UART3
   RCGCGPIO |= (1 << 0); // Enable PortA, PA4 is U3Rx, PA5 is U3Tx
   GPIOAFSEL_A = (1 << 4) | (1 << 5); //PA4, PA5 are used
   GPIOPCTL_A = (1 << 16) | (1 << 20); // Setup alternate function for PA4, PA5,4 bits for a pin
   GPIODEN_A = (1 << 4) | (1 << 5); // Digital function for PA4, PA5

   /*
   Baud-Rate = 9600
   Clock is setup to 60 MHZ
   BRD = 60000000 / (16 * 9600) = 390.625  (130, 32, 781, 78)
   UARTFBRD[DIVFRAC] = int(0.625 * 64 + 0.5) = 40  (13, 35, 16, 8)
   */
   UARTCTL_3 &= ~(1 << 0); 
   UARTIBRD_3 = 390; 
   UARTFBRD_3 = 40;
   UARTLCRH_3 = (0x3 << 5); // 8 bit, no parity, 1-bit stop bits
   UARTCC_3 = 0x0; // Select system clock
   UARTCTL_3 = (1 << 0) | (1 << 8) | (1 << 9); // Enables UART3, 8 and 9 are for receive and transmition
};

void Send_Char(char data) {
   while ((UARTFR_3 & (1 << 5)) != 0); // wait until finished transmitting previous data
   UARTDR_3 = data; // writes c into data
}

void Send_sentance(char* sentance) {
   while (*sentance) {  // while sentance is not pointing at \0
      Send_Char(*sentance);
      sentance += 1; 
   }
}

char Read_Char() {
   while ((UARTFR_3 & (1 << 4)) != 0); // wait until received some data
   return UARTDR_3; 
}