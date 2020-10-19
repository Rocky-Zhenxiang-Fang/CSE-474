#ifndef __HEADER1_H__
#define __HEADER1_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define GPIOAMSEL_C (*(( volatile uint32_t *) 0x4005A528 ))
#define GPIODIR_C (*(( volatile uint32_t *) 0x4005A400 ))
#define GPIODEN_C (*(( volatile uint32_t *) 0x4005A51C ))
#define GPIOAFSEL_C (*(( volatile uint32_t *) 0x4005A420 ))
#define GPIODATA_C (*(( volatile uint32_t *) 0x4005A3FC ))

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))

#define GPIODIR_J (*((volatile uint32_t *)0x40060400))
#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))
#define GPIOLOCK_J (*((volatile uint32_t *)0x40060520))
#define GPIOCR_J (*((volatile uint32_t *)0x40060524))
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC))

#define GPIODIR_L (*((volatile uint32_t *)0x40062400))
#define GPIOPUR_L (*((volatile uint32_t *)0x40062510))
#define GPIOLOCK_L (*((volatile uint32_t *)0x40062520))
#define GPIOCR_L (*((volatile uint32_t *)0x40062524))
#define GPIODEN_L (*((volatile uint32_t *)0x4006251C))
#define GPIODATA_L (*((volatile uint32_t *)0x400623FC))
#define GPIOAMSEL_L (*((volatile uint32_t *)0x40062528))
#define GPIOAFSEL_L (*((volatile uint32_t *)0x40062420))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIOAMSEL_N (*((volatile uint32_t *)0x40064528))
#define GPIOAFSEL_N (*((volatile uint32_t *)0x40064420))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))

#endif //__HEADER1_H__
