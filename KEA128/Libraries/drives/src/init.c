#include "init.h"
#include "isr.h"
#define CoderDelay 10
/*
  adc_init(ADC0_SE4);//B0
  adc_init(ADC0_SE5);//B1
  adc_init(ADC0_SE6);//B2
  adc_init(ADC0_SE7);//B3
  adc_init(ADC0_SE12);//F4
  adc_init(ADC0_SE13);//F5
  adc_init(ADC0_SE14);//F6
  adc_init(ADC0_SE15);//F7
*/

//������ָ���ȡ���ڲ����ã�
 unsigned int encoder_get(unsigned char select)
{
  unsigned char k;
  unsigned int dat1=0;
  if(select==0) 
  {
    CSnR0;
    delay_us(CoderDelay);  
    for(k=0;k<10;k++)
    { 
      CLKR0;
      delay_us(CoderDelay);
      CLKR1;
      delay_us(CoderDelay);
      dat1=(dat1<<1)+DATER;
    }  
    for(k=0;k<6;k++)
    {
      CLKR0;
      delay_us(CoderDelay);
      CLKR1;
      delay_us(CoderDelay);
    }
    CSnR1;
  }
  else 
  {
    CSnL0;
    delay_us(CoderDelay);  
    for(k=0;k<10;k++)
    {
      CLKL0;
      delay_us(CoderDelay);
      CLKL1;
      delay_us(CoderDelay);
      dat1=(dat1<<1)+DATEL;
    }  
    for(k=0;k<6;k++)
    {
      CLKL0;
      delay_us(CoderDelay);
      CLKL1;
      delay_us(CoderDelay);
    }     
    CSnL1;
  }
  return dat1;
}

//���ص�ǰ�ٶ�ֵ
 int16 Speed_Get(unsigned char select)
{
  static int16 L_num[2]={0},N_num[2]={0} ;  //����ٶ�ָ�����ݣ��ڲ����ã�
  int16 V;
  L_num[select]=N_num[select]; //�ϴ��ϴ�λ������
  N_num[select]=encoder_get(select);  //��ȡ��ǰλ������ 
  V = (N_num[select]-L_num[select]);  
  if(V>512) 
   V = V-1023;
  if(V<-512)
   V = V+1023;
  if(select==0) V=-1*V;
  return V;
}

void delay_us(unsigned int delay)
{
	while(delay--)
	{
		asm("NOP");
	}
}





void sys_init(void)
{ 
  gpio_init (E5,GPO,0);//������
  
  gpio_init (E4,GPI,1);//�ɻɹ�
  
  //gpio_init (I3,GPI,0);
  
  FLASH_Init();//FLASH��ʼ��
 
  ///////////////����///////////
  gpio_init (H6,GPI,0);//��
  gpio_init (D0,GPI,0);//��
  gpio_init (H7,GPI,0);//��
  gpio_init (G1,GPI,0);//��
  gpio_init (G3,GPI,0);//����
  
  ///////////adc��/////////////
  /*adc_init(ADC0_SE4);//B0
  adc_init(ADC0_SE5);//B1
  adc_init(ADC0_SE7);//F5
  adc_init(ADC0_SE15);//F7*/
  
  adc_init(ADC0_SE4);//B0
  adc_init(ADC0_SE5);//B1
  adc_init(ADC0_SE6);//B2
  adc_init(ADC0_SE7);//B3
  adc_init(ADC0_SE12);//F4
  adc_init(ADC0_SE13);//F5
  adc_init(ADC0_SE14);//F6
  adc_init(ADC0_SE15);//F7
  
  OLED_Init();//��ʾ��
  
  uart_init(DEBUG_PORT,DEBUG_BAUD);//��ʼ������
  
  ftm_pwm_init(ftm2,ftm_ch0,17000,0);//PWM0 PTH0����zuofan
  ftm_pwm_init(ftm2,ftm_ch1,17000,0);//PWM1 PTH1��youfan
  ftm_pwm_init(ftm2,ftm_ch2,17000,0);//PWM0 PTB4����youzhen
  ftm_pwm_init(ftm2,ftm_ch3,17000,0);//PWM1 PTB5�ҷ�zupozhen
  
  gpio_init(H5,GPI,1);//����   �����ʼ��
  gpio_init(H2,GPO,1);//���
  gpio_init(E7,GPO,1);//���
  gpio_init(C7,GPI,1);//����
  gpio_init(A0,GPO,1);//���
  gpio_init(A1,GPO,1);//���
  
  Speed_Get(0);
  Speed_Get(1);
  Speed_Get(0);
  Speed_Get(1);
  
  InitMPU6050();//������
  
  systick_delay_ms(200);
  
  pit_init(pit0,4);//4ms�ж�
  set_irq_priority(PIT_CH0_IRQn,1);//�������ȼ�1
  enable_irq(PIT_CH0_IRQn);//ʹ���ж�

  welcome();
  
  bisai       =    conf[SpeedSelect][1 ];//105-120
  SetSpeedMax =    conf[SpeedSelect][2 ];//110-120
  PoSpd       =    conf[SpeedSelect][3 ];
  anncnt[0]   =    conf[SpeedSelect][4 ];
  anncnt[1]   =    conf[SpeedSelect][5 ];
  anncnt[2]   =    conf[SpeedSelect][6 ];
  anncnt[3]   =    conf[SpeedSelect][7 ];
  anncnt[4]   =    conf[SpeedSelect][8 ];
  XieKp       =    conf[SpeedSelect][9 ];
  MidKp       =    conf[SpeedSelect][10];
  AdVKp  	  =    conf[SpeedSelect][11];
  tttsim      =    conf[SpeedSelect][12];//70
  ttttim      =    conf[SpeedSelect][13]; //220
  tttbim      =    conf[SpeedSelect][14];//70
  AnnLet      =    conf[SpeedSelect][15];
  AnnRit      =    conf[SpeedSelect][16];
  Annmb       =    conf[SpeedSelect][17];
  PoHuan      =    conf[SpeedSelect][18];//1�Ƿ�����
  shizibug    =    conf[SpeedSelect][19];//0��1�ǵ�����ʮ��bugʱʹ��      
  kpcha2      =    conf[SpeedSelect][20];//1Ϊ����յ���
  Endtim      =    conf[SpeedSelect][21];//��ʱ���ɻɹ�
  RunTim      =    conf[SpeedSelect][22];//ʱ��
  lowtim1     =    conf[SpeedSelect][23];//����ʱ��
  lowtim2     =    conf[SpeedSelect][24];//����ʱ��
  aa          =    conf[SpeedSelect][25];//wandaojiansu
  kp          =    conf[SpeedSelect][26];
  kd          =    conf[SpeedSelect][27];
  njnutim2    =    conf[SpeedSelect][28];
  njnutim1    =    conf[SpeedSelect][29];
 /* kp          =    conf[SpeedSelect][1 ];100
  kd          =    conf[SpeedSelect][2 ];110
  Endtime     =    conf[SpeedSelect][3 ];
  SetSpeedMax =    conf[SpeedSelect][4 ];280
  SpdKp       =    conf[SpeedSelect][5 ];190
  SpdKi       =    conf[SpeedSelect][6 ];5
  AnnKp       =    conf[SpeedSelect][7 ];
  AnnKd       =    conf[SpeedSelect][8 ];21
  Vkp         =    conf[SpeedSelect][9 ];160
  Vkd         =    conf[SpeedSelect][10];140
  KFC         =    conf[SpeedSelect][11]; 50
  ttttim      =    conf[SpeedSelect][12];38
  SpdAnnOut   =    conf[SpeedSelect][13];
  aa          =    conf[SpeedSelect][14];13
  bb          =    conf[SpeedSelect][15];3
  anncnt1     =    conf[SpeedSelect][16];
  anncnt2     =    conf[SpeedSelect][17];
  anncnt3     =    conf[SpeedSelect][18];
  anncnt4     =    conf[SpeedSelect][19];
  anncnt5     =    conf[SpeedSelect][20];
  moluntai    =    conf[SpeedSelect][21];*/
}
