/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/10/27
 * This header is used for Task_2_b in CSE 474 Au
 * */


#ifndef __Task_2_b_header_H__
#define __Task_2_b_header_H__

//GPIO 
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))

#define GPIODIR_J (*((volatile uint32_t *)0x40060400))
#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))
#define GPIOLOCK_J (*((volatile uint32_t *)0x40060520))
#define GPIOCR_J (*((volatile uint32_t *)0x40060524))
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC))
#define GPIOIS_J  (*((volatile uint32_t *)0x40060404))
#define GPIOIBE_J (*((volatile uint32_t *)0x40060408))
#define GPIOIEV_J (*((volatile uint32_t *)0x4006040C))
#define GPIOICR_J (*((volatile uint32_t *)0x4006041C))
#define GPIOIM_J (*((volatile uint32_t *)0x40060410))
#define GPIOMIS_J (*((volatile uint32_t *)0x40060418))


//Timer 
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

#define GPTMCTL_TIMER_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG_TIMER_0 (*((volatile uint32_t *)0x40030000))
#define GPTMTAMR_TIMER_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAILR_TIMER_0 (*((volatile uint32_t *)0x40030028))
#define GPTMRIS_TIMER_0  (*((volatile uint32_t *)0x4003001C))
#define GPTMICR_TIMER_0  (*((volatile uint32_t *)0x40030024))
#define GPTMIMR_TIMER_0  (*((volatile uint32_t *)0x40030018))

//Interrupt
#define NVIC_EN0  (*((volatile uint32_t *)0xE000E100))
#define NVIC_EN1  (*((volatile uint32_t *)0xE000E104))

//define MACRO
#define LED1 0x1 // PN1 
#define LED2 0x0  // PN0
#define SW1 0x0 // PJ0 
#define SW2 0x1 // PJ1

#define TATORIS 0x1

//Sets up GPIO Ports
void SetUpGPIO(void);

//Sets up timer
void SetUpTimer(void); 

#endif //__Task_2_b_header_H__
