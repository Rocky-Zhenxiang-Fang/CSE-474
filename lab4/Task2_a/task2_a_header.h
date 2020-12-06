/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This header is used for Task_2 in CSE 474 Au
 * */

#ifndef __Task_2_header_H__
#define __Task_2_header_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

//Port_L
#define GPIODIR_L (*((volatile uint32_t *)0x40062400))
#define GPIOPUR_L (*((volatile uint32_t *)0x40062510))
#define GPIOLOCK_L (*((volatile uint32_t *)0x40062520))
#define GPIOCR_L (*((volatile uint32_t *)0x40062524))
#define GPIODEN_L (*((volatile uint32_t *)0x4006251C))
#define GPIODATA_L (*((volatile uint32_t *)0x400623FC))
#define GPIOAMSEL_L (*((volatile uint32_t *)0x40062528))
#define GPIOAFSEL_L (*((volatile uint32_t *)0x40062420))
#define GPIOIS_L  (*((volatile uint32_t *)0x40062404))
#define GPIOIBE_L (*((volatile uint32_t *)0x40062408))
#define GPIOIEV_L (*((volatile uint32_t *)0x4006240C))
#define GPIOICR_L (*((volatile uint32_t *)0x4006241C))
#define GPIOIM_L (*((volatile uint32_t *)0x40062410))
#define GPIOMIS_L (*((volatile uint32_t *)0x40062418))

//Timer
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

//Timer_0
#define GPTMCTL_TIMER_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG_TIMER_0 (*((volatile uint32_t *)0x40030000))
#define GPTMTAMR_TIMER_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAILR_TIMER_0 (*((volatile uint32_t *)0x40030028))
#define GPTMRIS_TIMER_0  (*((volatile uint32_t *)0x4003001C))
#define GPTMICR_TIMER_0  (*((volatile uint32_t *)0x40030024))
#define GPTMIMR_TIMER_0  (*((volatile uint32_t *)0x40030018))
#define GPTMTAV_TIMER_0  (*((volatile uint32_t *)0x40030050))

//Timer_1
#define GPTMCTL_TIMER_1 (*((volatile uint32_t *)0x4003100C))
#define GPTMCFG_TIMER_1 (*((volatile uint32_t *)0x40031000))
#define GPTMTAMR_TIMER_1 (*((volatile uint32_t *)0x40031004))
#define GPTMTAILR_TIMER_1 (*((volatile uint32_t *)0x40031028))
#define GPTMRIS_TIMER_1  (*((volatile uint32_t *)0x4003101C))
#define GPTMICR_TIMER_1  (*((volatile uint32_t *)0x40031024))
#define GPTMIMR_TIMER_1  (*((volatile uint32_t *)0x40031018))

//Timer_2
#define GPTMCTL_TIMER_2 (*((volatile uint32_t *)0x4003200C))
#define GPTMCFG_TIMER_2 (*((volatile uint32_t *)0x40032000))
#define GPTMTAMR_TIMER_2 (*((volatile uint32_t *)0x40032004))
#define GPTMTAILR_TIMER_2 (*((volatile uint32_t *)0x40032028))
#define GPTMRIS_TIMER_2  (*((volatile uint32_t *)0x4003201C))
#define GPTMICR_TIMER_2  (*((volatile uint32_t *)0x40032024))
#define GPTMIMR_TIMER_2  (*((volatile uint32_t *)0x40032018))

//Interrupt
#define NVIC_EN0  (*((volatile uint32_t *)0xE000E100))
#define NVIC_EN1  (*((volatile uint32_t *)0xE000E104))

// useful MACRO
#define TATORIS 0x1
#define sysButton 0x0 // PL0
#define pedButton 0x1 // PL1

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// main excecution function, where finite state system is defined
void TickTrafficLight(void);

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

// Set up RCGCGPIO
void SetupGPIO(void); 

// Draws buttons on the LCD panel
void DrawUI(void);

// Set up external switches
void ExternSwitchInit(void);

// Set up LEDs
void LEDInit (void);

// Set up timer
void SetupTimer(void);

// Turn on the green LED
void GreenOn(void);

// Turn off the green LED
void GreenOff(void);

// Turn on the red LED
void RedOn(void);

// Turn off the red LED
void RedOff(void);

// Turn on the yellow LED
void YellowOn(void);

// Turn off the yellow LED
void YellowOff(void);

// Turn off the system, which is turn off all LEDs
void SysOff (void); 

#endif //__Task_2_header_H__
