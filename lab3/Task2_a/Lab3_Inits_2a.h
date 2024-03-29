/**
 * EE/CSE 474: Lab3 drivers header file
 */

// NOTE: All necessary functions for Task 1a have been declared in
// this header file, and thus you should not modify it for task 1a.
// However, you may modify it if you want to reuse it for other tasks.

#ifndef _Lab3_Inits_H_
#define _Lab3_Inits_H_

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

// Initializes the 2 onboard Switches.
void Switch_Init(void);

// Initializes ADC Module 0 Sample Sequencer 3. The module is triggered by
// Timer_0 and reads data from internal temperature sensor
void ADCReadPot_Init(void);

// Initializes Timer 0A to trigger ADC0 at 1 HZ.
void TimerADCTriger_Init(void);

// Initializes UART0 to transfer tempurature data to PC
void UART_Init(void);

// Sends a char c to computer using UART
void Send_Char(char c);

// Send sentance using Send_Char
void Send_sentance(char* sentance); 
#endif  // _Lab3_Inits_H_
