#include "KEA128_kbi.h"
KBI_Type * kbi[2] = KBI_BASES;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      KBI�����жϳ�ʼ��
//  @param      chn             ѡ��kbiͨ��   ��ѡ��Χ �ο�KBI_CHnö��
//  @return     void
//  @since      v2.0
//  Sample usage:               kbi_init(KBI1_P0,IRQ_RISING);		            //ͨ��ѡ��ΪKBI1_P0�������ش���
//								set_irq_priority(KBI1_IRQn,1);					//�������ȼ�,�����Լ����������� �����÷�ΧΪ 0 - 3
//								enable_irq(KBI1_IRQn);							//��KBI1_IRQn���жϿ���
//								EnableInterrupts;								//���ܵ��жϿ���
//-------------------------------------------------------------------------------------------------------------------
void kbi_init(KBI_CHn chn, TRIG_CFG cfg)
{
    uint8 kbi_x,kbi_n;
    
    kbi_x = KBIx(chn);      //��ȡģ���
    kbi_n = KBIn(chn);      //��ȡ���ź�
    
    //����KBIʱ��
    if(0 == kbi_x)  SIM->SCGC |= SIM_SCGC_KBI0_MASK;
    else            SIM->SCGC |= SIM_SCGC_KBI1_MASK;
    
    //����KBI�ж�
    kbi[kbi_x]->SC &= ~(uint32)KBI_SC_KBIE_MASK;

    //���ô���ģʽ
    if((IRQ_FALLING == cfg) || (IRQ_FALLING_LOW == cfg))
        kbi[kbi_x]->ES &= ~((uint32)1<<kbi_n);
    else
        kbi[kbi_x]->ES |= ((uint32)1<<kbi_n);
    
    //��������
    port_pull((PTX_n)((uint8)(chn)));
    
    //ʹ��KBI����
    kbi[kbi_x]->PE |= (1<<kbi_n);
    
    if(0 == kbi_x)  CLEAN_KBI0_FLAG;                         //�����־λ
    else            CLEAN_KBI1_FLAG;                         //�����־λ
    
    //���ô���ģʽ
    if((IRQ_FALLING == cfg) || (IRQ_RISING == cfg))
    {
        kbi[kbi_x]->SC = 0;                                 //���ش���ģʽ 0�����ش���   1����ƽ����
    }
    else
    {
        kbi[kbi_x]->SC = KBI_SC_KBMOD_MASK;                 //���ش���ģʽ 0�����ش���   1����ƽ����
    }
    
    
    
    kbi[kbi_x]->SC |= ( 0
                        | KBI_SC_KBIE_MASK    //KBI�ж�ʹ��
                        | KBI_SC_RSTKBSP_MASK
                        //| KBI_SC_KBMOD_MASK   //���ش���ģʽ 0�����ش���   1����ƽ����
                        | KBI_SC_KBSPEN_MASK  //ʹ��KBI_SP�Ĵ���
                        );
}




