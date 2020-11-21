/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/16
 * This header is used for Lab3_Task_2_b in CSE 474 Au
 * */

#ifndef __Task_2_b_header_H__
#define __Task_2_b_header_H__

// ******************** CLOCK CONTROL REGISTERS ********************** //
#define MOSCCTL                 (*(volatile uint32_t *) 0x400FE07C)
#define RIS                     (*(volatile uint32_t *) 0x400FE050)
#define RSCLKCFG                (*(volatile uint32_t *) 0x400FE0B0)
#define PLLFREQ0                (*(volatile uint32_t *) 0x400FE160)
#define PLLFREQ1                (*(volatile uint32_t *) 0x400FE164)
#define PLLSTAT                 (*(volatile uint32_t *) 0x400FE168)
#define MEMTIM0                 (*(volatile uint32_t *) 0x400FE0C0)

// ******************** GPIO REGISTERS ********************** //
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define GPIOAFSEL_A (*((volatile uint32_t *)0x40058420))
#define GPIOPCTL_A (*((volatile uint32_t *)0x4005852C))
#define GPIODEN_A  (*((volatile uint32_t *)0x4005851C))

// ******************** UART REGISTERS ********************** //
#define RCGCUART (*((volatile uint32_t *)0x400FE618))

#define UARTCTL_3 (*((volatile uint32_t *)0x4000F030))
#define UARTIBRD_3 (*((volatile uint32_t *)0x4000F024))
#define UARTFBRD_3 (*((volatile uint32_t *)0x4000F028))
#define UARTLCRH_3 (*((volatile uint32_t *)0x4000F02C))
#define UARTCC_3 (*((volatile uint32_t *)0x4000FFC8))
#define UARTFR_3 (*((volatile uint32_t *)0x4000F018))
#define UARTDR_3 (*((volatile uint32_t *)0x4000F000))

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

// Initializes UART0 to transfer tempurature data to PC
void UART_Init(void);

// Sends a char c to computer using UART
void Send_Char(char c);

// Send sentance using Send_Char
void Send_sentance(char* sentance); 

// Reads char from UART to the program
char Read_Char(); 


#endif //__Task_2_a_header_H__
