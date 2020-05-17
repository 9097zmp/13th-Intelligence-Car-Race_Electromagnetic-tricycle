#include "KEA128_irq.h"
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ⲿ�ж�
//  @param      void            
//  @return     void
//  @since      v2.0
//  Sample usage:               irq_init();
//								set_irq_priority(IRQ_IRQn,1);					//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3
//								enable_irq(IRQ_IRQn);							//��pit0���жϿ���
//								EnableInterrupts;								//���ܵ��жϿ���  
//  @note                       ��KEA128_port_cfg�ļ��л����ţ�irqͬһʱ��ֻ�ܶ�һ�����Ž��м��
//                              �����жϺ��������ʹ��CLEAR_IRQ_FLAG�궨������жϱ�־λ
//-------------------------------------------------------------------------------------------------------------------

void irq_init(void)
{
    uint8 ptn;
    uint32 temp;

    if(A5 == IRQ_PIN)
    {
        temp = SIM->PINSEL;
        temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
        SIM->PINSEL = temp | SIM_PINSEL_IRQPS(0);
    }
    else
    {
        ptn = PTn(IRQ_PIN);//��ȡ���ź�
        
        temp = SIM->PINSEL;
         temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
         SIM->PINSEL = temp | SIM_PINSEL_IRQPS(ptn+1);
    }
    
    SIM->SCGC |= SIM_SCGC_IRQ_MASK;     //����IRQʱ��
    temp = (0
            //| IRQ_SC_IRQPDD_MASK      //1�������������� 0��ʹ����������  ע�ʹ���0
            | IRQ_SC_IRQEDG_MASK        //1�������ػ��߸ߵ�ƽ���� 0���½��ػ��ߵ͵�ƽ����   ע�ʹ���0
            | IRQ_SC_IRQPE_MASK         //ʹ��IRQ
            | IRQ_SC_IRQIE_MASK         //�����ж�
            //| IRQ_SC_IRQMOD_MASK      //1�����������ƽ����  0��ֻ�����ش���  ע�ʹ���0
            );
    IRQ->SC = temp;
    
}





