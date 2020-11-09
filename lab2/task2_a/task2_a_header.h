/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/27
 * This header is used for Task_1_a in CSE 474 Au
 * */


#ifndef __Task_1_a_header_H__
#define __Task_1_a_header_H__

//GPIO 
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))


//Timer 
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

#define GPTMCTL_TIMER_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG_TIMER_0 (*((volatile uint32_t *)0x40030000))
#define GPTMTAMR_TIMER_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAILR_TIMER_0 (*((volatile uint32_t *)0x40030028))
#define GPTMRIS_TIMER_0  (*((volatile uint32_t *)0x4003001C))
#define GPTMICR_TIMER_0  (*((volatile uint32_t *)0x40030024))
#define GPTIMR_TIMER_0  (*((volatile uint32_t *)0x40030018))

//Interrupt
#define NVIC_EN0  (*((volatile uint32_t *)0xE000E100))

//define MACRO
#define LED1 0x2  // PN1 
#define LED2 0x1  // PN0
#define LED3 0x10 // PF4
#define LED4 0x1  // PF0 
#define TATORIS 0x1

//Sets up GPIO Ports
void SetUpGPIO(void);

//Sets up timer
void SetUpTimer(void); 

//Used to turn on/off the approiate LED based on flag
void SwitchLEDs(void); 


#endif //__Task_1_a_header_H__
