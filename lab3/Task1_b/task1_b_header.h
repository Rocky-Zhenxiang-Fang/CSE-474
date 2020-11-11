/**
 * Rocky Zhenxiang Fang
 * Student ID: 1926438
 * 2020/11/09
 * This header is used for Task_1_a in CSE 474 Au
 * */

#ifndef __Task_1_b_header_H__
#define __Task_1_b_header_H__

//GPIO 
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

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

// ADC
#define RCGCADC (*((volatile uint32_t *)0x400FE638))

#define ADCCC_ADC0 (*((volatile uint32_t *)0x40038FC8))
#define ADCACTSS_ADC0 (*((volatile uint32_t *)0x40038000))
#define ADCEMUX_ADC0  (*((volatile uint32_t *)0x40038014))
#define ADCSSMUX3_ADC0  (*((volatile uint32_t *)0x400380A0))
#define ADCSSCTL3_ADC0  (*((volatile uint32_t *)0x400380A4))
#define ADCIM_ADC0  (*((volatile uint32_t *)0x40038008))
#define ADCISC_ADC0  (*((volatile uint32_t *)0x4003800C))
#define ADCSSFIFO3_ADC0  (*((volatile uint32_t *)0x400380A8))

// Timer 
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

#define GPTMCTL_TIMER_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG_TIMER_0 (*((volatile uint32_t *)0x40030000))
#define GPTMTAMR_TIMER_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAILR_TIMER_0 (*((volatile uint32_t *)0x40030028))
#define GPTMRIS_TIMER_0  (*((volatile uint32_t *)0x4003001C))
#define GPTMICR_TIMER_0  (*((volatile uint32_t *)0x40030024))
#define GPTIMR_TIMER_0  (*((volatile uint32_t *)0x40030018))
#define GPTMADCEV_TIMER_0  (*((volatile uint32_t *)0x40030070))

//Interrupt
#define NVIC_EN0  (*((volatile uint32_t *)0xE000E100))
#define NVIC_EN1  (*((volatile uint32_t *)0xE000E104))

//define MACRO
#define ADC0 0  // bit 0
#define AIN2 1 // PE1
#define ASEN3 3 // at bit 3
#define EM3 12 // 4 bit field, lsb at 12
#define SW1 0x0 // PJ0 
#define SW2 0x1 // PJ1

#define TATORIS 0x1

//Sets up GPIO Ports
void SetUpGPIO(void);

//Sets up timer
void SetUpTimer(void); 

//Used to turn on/off the approiate LED based on flag
void SwitchLEDs(void); 


#endif //__Task_1_b_header_H__
