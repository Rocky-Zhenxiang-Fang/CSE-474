/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/23
 * This header is used for Task_1_b in CSE 474 Au
 * */

#ifndef __Task_1_b_header_H__
#define __Task_1_b_header_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N_1 (*((volatile uint32_t *)0x40064308))
#define GPIODATA_N_0 (*((volatile uint32_t *)0x40064004))

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F_0 (*((volatile uint32_t *)0x4005D004))
#define GPIODATA_F_4 (*((volatile uint32_t *)0x4005D040))

#define GPIODIR_J (*((volatile uint32_t *)0x40060400))
#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))
#define GPIOLOCK_J (*((volatile uint32_t *)0x40060520))
#define GPIOCR_J (*((volatile uint32_t *)0x40060524))
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC))

// Set up RCGCGPIO
void SetUp(void);

// Set up onboard switches
void SetSwitches(void); 

// Set up onboard LEDs
void SetLEDs(void); 

// main excution function, reads switch input and ligh LED
void LightLED(void);

#endif //__Task_1_b_header_H__
