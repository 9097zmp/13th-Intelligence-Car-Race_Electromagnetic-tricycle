/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�ж��ļ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#include "isr.h"
#include "KEA128_adc.h"
#include "math.h"
#include "KEA128_ftm.h"
#include "init.h"

uint32  TimeRunCount=0;
int32   GyroX=0,GyroXo=0,GyroY=0,GyroYo=0;         //�����ǲɼ�ֵ
int32   Acc_Z=0;                                   //���ٶȼƲɼ�ֵ
int32   Gyropitchfloat=0,Gyrorollfloat=0,Anglefloat=0;//ֱ�����������,ת�����������,�Ƕ����
int16   PoHuan=0;//00000��ʾȫ�̿�����00001��ʾ��һ�����رգ�00011=3��ʾ��һ�������رգ�

void PIT_CH0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0);
	
	static uint16 speedouttim=0;
	
	SpdGet();
	ErrorGet();
	SpdControl();
	SpdDuty();

	
	/*ftm_pwm_duty(ftm2,ftm_ch0,400);//��zhen40
	ftm_pwm_duty(ftm2,ftm_ch3,0);//��fan40
	ftm_pwm_duty(ftm2,ftm_ch1,0);//��zhen70
	ftm_pwm_duty(ftm2,ftm_ch2,0);//��fan65*/
	//Uart_send();
    
}
void PIT_CH1_IRQHandler(void)
{
    PIT_FlAG_CLR(pit1);

    
}

void IRQ_IRQHandler(void)
{
    CLEAR_IRQ_FLAG;
    
}


void KBI0_IRQHandler(void)
{
    CLEAN_KBI0_FLAG;
    
}





/*
�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ���������ڶ�ʱ�� ͨ��0���ж�
void PIT_CH0_IRQHandler(void)
{
    ;
}
�ǵý����жϺ������־λ

FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/



