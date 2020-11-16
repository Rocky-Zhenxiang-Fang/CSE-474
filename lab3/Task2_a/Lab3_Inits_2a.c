/**
 * EE/CSE 474: Lab3 drivers starter code
 */

#include "PLL_Header.h"
#include "Lab3_Inits_2a.h"

// STEP 0a: Include your header file here
// YOUR CUSTOM HEADER FILE HERE
#include "task2_a_header.h"

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

void Switch_Init(void) {
   volatile unsigned short delay = 0;
   RCGCGPIO |= 0x1100; // Enable PortJ(Switches) and PortN(led1, 2) GPIO
   delay++; // Delay 2 more cycles before access Timer registers
   delay++; // Refer to Page. 756 of Datasheet for info

   GPIODIR_J = 0x0; // Set all PJ pins to input
   GPIOLOCK_J = 0x4C4F434B; // Unlock Port J
   GPIOCR_J = 0x3; // Allows GPIOPUR and GPIODEN to be used
   GPIOPUR_J = 0x3; // Select PJ0, PJ1 to pull up
   GPIODEN_J = 0x3; // Set PJ0 and PJ1 to digital port

   // Prot_J (Switches) Interrupt
   GPIOIS_J &= ~0x3; // make switches edge sensitive
   GPIOIBE_J &= ~0x3; // Only detect one edge, selected in IEV
   GPIOIEV_J = ~0x3; // detect on falling edge
   GPIOICR_J |= 0x3; // clear prior interrupt
   GPIOIM_J |= 0x3; // unmask interrupt
   NVIC_EN1 |= (0x1 << 19); // Enables Port I interrupt. Set field 19 to 1
}

void ADCReadPot_Init(void) {
   // STEP 2: Initialize ADC0 SS3.
   // 2.1: Enable the ADC0 clock
   RCGCADC |= (1 << ADC0); // set bit for ADC0 to 1 

   // 2.2: Delay for RCGCADC (Refer to page 1073)
   unsigned char delay = 0; 
   delay ++; 
   delay ++; 
   delay ++;  // delay for three system clocks

   // 2.3: Power up the PLL (if not already)
   PLLFREQ0 |= 0x00800000; // we did this for you

   // 2.4: Wait for the PLL to lock
   while (PLLSTAT != 0x1); // we did this for you

   // 2.5: Configure ADCCC to use the clock source defined by ALTCLKCFG
   ADCCC_ADC0 = 0x1; 
   
   // 2.11: Disable sample sequencer 3 (SS3)
   ADCACTSS_ADC0 &= ~(1 << ASEN3); 

   // 2.12: Select timer as the trigger for SS3
   ADCEMUX_ADC0 = (0x5 << EM3); // set SS3 to timer 

   // 2.13: Select the temperature seneor for SS3
   ADCSSCTL3_ADC0 |= (1 << 3);
   ADCSSTSH3_ADC0 = 0x6;    

   // 2.14: Configure ADCSSCTL3 register
   ADCSSCTL3_ADC0 |= 0x6; // 0110, enable inturrpt, set last bit
   
   // 2.15: Set the SS3 interrupt mask
   ADCIM_ADC0 |= (1 << 3); // using SS3

   // 2.16: Set the corresponding bit for ADC0 SS3 in NVIC
   ADCISC_ADC0 |= (1 << 3); // clears previous inturrpt
   NVIC_EN0 |= (0x1 << 17); // Enables ADC0 SS3 interrupt. Set field 17 to 1

   // 2.17: Enable ADC0 SS3
   ADCACTSS_ADC0 |= (1 << ASEN3); 
}

void TimerADCTriger_Init(void) {
   // STEP 3: Initialize Timer0A to trigger ADC0 at 1 HZ.
   // Hint: Refer to section 13.3.7 of the datasheet
   //step 1
   RCGCTIMER |= (1<<0); // Enables GPtimer 0, sets 1 at field 0

   //step 2
   GPTMCTL_TIMER_0 &= ~(1<<0); //Disables timer 0_A, set 0 to field 0
   GPTMCTL_TIMER_0 &= ~(1<<8); //Disables timer 0_B, set 0 to field 8

   //step 3
   GPTMCFG_TIMER_0 = 0x00000000; 

   //step 4
   GPTMCFG_TIMER_0 = 0x0; // select 32-bit mode

   //step 5
   GPTMTAMR_TIMER_0 |= (0x2<<0); 

   //step 6
   GPTMTAMR_TIMER_0 &= ~(1<<4); //Set TACDIR bit to 0 to count down, field 4

   //step 7
   GPTMTAILR_TIMER_0 = 60000000; 

   // step 8, control ADC0 SS3 by timer_0
   GPTMCTL_TIMER_0 |= (1<<5);
   GPTMADCEV_TIMER_0 |= (1 << 0); // trigger ADC when time out

   //step 9
   
   GPTMCTL_TIMER_0 |= (1<<0); // Enables timer 0_A
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
   Clock is setup to 60 MHZ
   BRD = 60000000 / (16 * 9600) = 390.625  (130, 32, 781, 78)
   UARTFBRD[DIVFRAC] = int(0.625 * 64 + 0.5) = 40  (13, 35, 16, 8)
   */

   // 6. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
   UARTCTL_0 &= ~(1 << 0); 

   // 7. Write the integer portion of the BRD to the UARTIBRD register.
   UARTIBRD_0 = 390; 

   // 8. Write the fractional portion of the BRD to the UARTFBRD register.
   UARTFBRD_0 = 40;

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

