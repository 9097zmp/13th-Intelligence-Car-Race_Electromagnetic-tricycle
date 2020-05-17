#ifndef __INIT_H   
#define __INIT_H 
#include "common.h"
#define CLKL0                    gpio_set(H2,0) //H2����͵�ƽ
#define CLKL1                    gpio_set(H2,1) //H2����ߵ�ƽ
#define CSnL0                    gpio_set(E7,0) //E7����͵�ƽ
#define CSnL1                    gpio_set(E7,1) //E7����ߵ�ƽ

#define CLKR0                    gpio_set(A0,0)    //A0����͵�ƽ
#define CLKR1                    gpio_set(A0,1)   //A0����ߵ�ƽ
#define CSnR0                    gpio_set(A1,0)    //A1����͵�ƽ
#define CSnR1                    gpio_set(A1,1)    //A1����ߵ�ƽ

#define DATEL                   gpio_get(H5)  //��ȡH5
#define DATER                   gpio_get(C7)  //��ȡD4

#define ring_on()               gpio_set(E5,1)   //E2����ߵ�ƽ   
#define ring_off()              gpio_set(E5,0)   //E2����͵�ƽ
void sys_init(void);
void delay_us(unsigned int delay);
int16 Speed_Get(unsigned char select);
#endif  //__INIT_H