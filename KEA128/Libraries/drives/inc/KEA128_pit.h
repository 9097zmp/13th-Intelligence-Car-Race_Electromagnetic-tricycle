#ifndef _KEA128_pit_h
#define _KEA128_pit_h
#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"
//����PITģ���
typedef enum
{
    pit0,
    pit1,
} PITn;

#define     PIT_FlAG_CLR(PITn)          PIT->CHANNEL[PITn].TFLG |= PIT_TFLG_TIF_MASK        //���жϱ�־��д1 ��ձ�־λ��

void    pit_init(PITn pitn, uint32 cnt);
void    pit_delay(PITn pitn, uint32 cnt);
void    pit_time_start(PITn pitn);
uint32  pit_time_get(PITn pitn);
void    pit_close(PITn pitn);

//------------------------------------���´�������PIT�ж�------------------------------------
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * bus_clk_khz);                  //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * bus_clk_khz/1000);             //��ʼ��PITn�������ö�ʱʱ��(��λΪ us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * bus_clk_khz/1000000);          //��ʼ��PITn�������ö�ʱʱ��(��λΪ ns)


//------------------------------------���´�������PIT��ʱ------------------------------------
#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * bus_clk_khz);                //PIT��ʱ ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * bus_clk_khz/1000);           //PIT��ʱ us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * bus_clk_khz/1000000);        //PIT��ʱ ns(40M����Ƶ��ʱ��С��λΪ25ns)


#endif
