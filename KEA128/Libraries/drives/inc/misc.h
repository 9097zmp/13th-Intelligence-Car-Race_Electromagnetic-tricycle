#ifndef _misc_h
#define _misc_h
#include "common.h"
void write_vtor (int);                                              //�����ж�������ĵ�ַ                                                                                                                                   
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //ʹ��IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //��ֹIRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //�������ȼ������ȼ���Χ0-3                                                                                                                                       
#define EnableInterrupts                __enable_irq()              //ʹ��ȫ���ж�
#define DisableInterrupts               __disable_irq()             //��ֹȫ���ж�
#endif
