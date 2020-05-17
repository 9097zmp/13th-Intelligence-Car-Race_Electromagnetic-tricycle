#include "welcom.h"
#include "KEA128_adc.h"
#include "math.h"
#include "KEA128_ftm.h"
uint8    imgmode=0;                   //ģʽѡ��--->Runmode  Showmode
uint8    SpeedSelect=0;               //�ٶȵ�λ--High-Speed   Mid-Speed  Low-Speed
uint8    SaveMenu[ConfigNum][10];     //�˵��洢����
int16 SaveStep[ConfigNum];         //���洢����
void Main_Menu(void);             //�ٶȵ�λ��ʾ������
void Add_Menu(uint8 *menu,int16 step);//���Ӳ���
void Next_Menu(uint8 pos);            //�ڶ�����
void strcopy(uint8 *str1,uint8 *str2);   //�ַ�������
uint8 read_key(void)
{
  uint8 num=0,count=0;
  if(!gpio_get(H6)) {num+=1;count++;}//��
  if(!gpio_get(D0)) {num+=2;count++;}//��
  if(!gpio_get(H7)) {num+=3;count++;}//��
  if(!gpio_get(G1)) {num+=4;count++;}//��
  if(!gpio_get(G3)) {num+=5;count++;}//����
  if(count>=2) return 0;
  else return num; 
}
void welcome(void)                
{
  precursors_logo();
  systick_delay_ms(200);
  int16  pos=0;                 //*λ��
  int16  disppos=0;             //��ʾҳ��
  int16  DelayTime=0;           //������ʱʱ��
  uint8  Keycount=0;            //ȷ�ϼ�����
  uint8  Key=0;                 //������ȡ����
  uint8  NextFlag=0;            //ȷ�ϱ�־
  uint8  ReadMode=0;            //0------SD_Read,1------Flash_Read
  
    Add_Menu("1.bisai",1); //0��1             
    Add_Menu("2.SetSpd",1); //285��260-290��            
    Add_Menu("3.PoSpd",5); //220              
    Add_Menu("4.huan1",1);              
    Add_Menu("5.huan2",1);            
    Add_Menu("6.huan3",1);              
    Add_Menu("7.huan4",1);               
    Add_Menu("8.huan5",1);              
    Add_Menu("9.XieKp",2);               
    Add_Menu("10.MidKp",2);          
    Add_Menu("11.AdVKp",2);        
    Add_Menu("12.tttsim",2);//60           
    Add_Menu("13.ttttim",2);//68         
    Add_Menu("14.tttbim",2);//70         
    Add_Menu("15.AnnLet",1);//20
    Add_Menu("16.AnnRit",1);//50          
    Add_Menu("17.Annmb",1);//35             
    Add_Menu("18.PoHuan",1);//1�����              
    Add_Menu("19.shzbug",1);//0
    Add_Menu("20.stpcar",1); //1��0Ϊ��ͣ����
    Add_Menu("21.ghgtim",100);          
    Add_Menu("22.RunTim",50);               
    Add_Menu("23.lowtm1",100);               
    Add_Menu("24.lowtm2",100);               
    Add_Menu("25.aa",1);//6              
    Add_Menu("26.kp",5);//120          
    Add_Menu("27.kd",5);//110              
    Add_Menu("28.PoDly",10);//450
    Add_Menu("dbstptim",10);//70
  
    
  //���ν�����Ӳ���-----------------  
	 flashconf_read();
	 XieKp   =    conf[SpeedSelect][9];
	 MidKp   =    conf[SpeedSelect][10];
	 AdVKp   =    conf[SpeedSelect][11];
    Main_Menu();                  //���ֵand���˵�  
  while(!NextFlag)
 {
  Key=read_key();//��ȡ����
  if(Key==0)continue;//������ȡ
  else
  {
    switch(Key)
    {
    case 1:pos--;if(pos<0)pos=0;OLED_fill(6,8,0x00);OLED_P6x8Str(0,pos*2,"*");break;
    case 2:pos++;if(pos>3)pos=3;OLED_fill(6,8,0x00);OLED_P6x8Str(0,pos*2,"*");break;
    case 5:if(pos<3)SpeedSelect=pos;else ReadMode=1;NextFlag=1;pos=0;break;
    }
    systick_delay_ms(200);
  }
 }

 //����������-------------------------------------------
  NextFlag=0;
  Next_Menu(disppos);
  OLED_P6x8Str(0,0,"*");
  while(!NextFlag)
  {
    Key=read_key();
    if(Key==0) {DelayTime=150;continue;}
    else
    {
      switch(Key)
      {
        case 2:Keycount=0;pos++;if(pos>7){pos=7;disppos++;if(disppos>(ConfigNum-8)) disppos=(ConfigNum-8);Next_Menu(disppos);}OLED_fill(6,8,0x00);OLED_P6x8Str(0,pos,"*");break;
        case 1:Keycount=0;pos--;if(pos<0){pos=0;disppos--;if(disppos<0)  disppos=0; Next_Menu(disppos);}OLED_fill(6,8,0x00);OLED_P6x8Str(0,pos,"*");break;
        case 4:Keycount=0;conf[SpeedSelect][pos+disppos]+=SaveStep[pos+disppos];Next_Menu(disppos);OLED_P6x8Str(0,pos,"*");break;
        case 3:Keycount=0;conf[SpeedSelect][pos+disppos]-=SaveStep[pos+disppos];if(conf[SpeedSelect][pos+disppos]<0) conf[SpeedSelect][pos+disppos]=0;Next_Menu(disppos);OLED_P6x8Str(0,pos,"*");break;
        case 5:Keycount++;if(Keycount>=2) {NextFlag=1;OLED_Fill(0x00);break;}     
      }
      systick_delay_ms(DelayTime);       //������ʱ
      DelayTime-=20;
      if(DelayTime<50) DelayTime=50;
    }
  } 
  flashconf_write();
}

//���ֵ���˵�
uint16 adc[8]={0};
uint16 xianshicanshu[8];
void Main_Menu(void)
{
  OLED_Fill(0x00);//����
  while(1)
  {
    adc[0]=adc_once(ADC0_SE4,ADC_12bit)/100.0f*AdVKp;//zuoxie
    adc[1]=adc_once(ADC0_SE5,ADC_12bit);//
    adc[2]=adc_once(ADC0_SE6,ADC_12bit)/100.0f*XieKp;//zuoshu
    adc[3]=adc_once(ADC0_SE7,ADC_12bit)/100.0f*MidKp;
    adc[4]=adc_once(ADC0_SE12,ADC_12bit)/100.0f*XieKp;//youxie
    adc[5]=adc_once(ADC0_SE13,ADC_12bit)/100.0f*AdVKp;//youheng
    adc[6]=adc_once(ADC0_SE14,ADC_12bit);//youshu
    adc[7]=adc_once(ADC0_SE15,ADC_12bit);//
	
	xianshicanshu[0]=fabs(1000.0f*(adc[2]-adc[7])/(adc[2]+adc[7]));
    xianshicanshu[1]=fabs(1000.0f*(adc[0]-adc[4])/(adc[2]+adc[7]));
	OLED_Print_Num(0,0,adc[1]);
    OLED_Print_Num(0,2,adc[0]);//
    OLED_Print_Num(0,4,adc[3]);//
    OLED_Print_Num(0,6,adc[2]);//
    OLED_Print_Num(56,0,adc[7]);//
    OLED_Print_Num(56,2,adc[5]);//
   // OLED_Print_Num(56,4,adc[6]);//
    OLED_Print_Num(56,6,adc[4]);//
	
	//xie shu heng
	
   if(5==read_key()) break; //������������������¸�����
   systick_delay_ms(50);
  }
   systick_delay_ms(200);
   OLED_Fill(0x00);
  while(1)
  {
    adc[0]=gpio_get(I3);
    adc[1]=adc_once(ADC0_SE5,ADC_12bit);
    adc[2]=adc_once(ADC0_SE6,ADC_12bit);
    adc[3]=adc_once(ADC0_SE7,ADC_12bit);
    adc[4]=adc_once(ADC0_SE12,ADC_12bit);//б�����ʵֵ
    adc[5]=adc_once(ADC0_SE13,ADC_12bit);
    adc[6]=adc_once(ADC0_SE14,ADC_12bit);
    adc[7]=adc_once(ADC0_SE15,ADC_12bit);
	  
	xianshicanshu[0]=gpio_get(E4);
    xianshicanshu[1]=1900.0f/adc[4] * 100.0f;
	
	 xianshicanshu[2]=1970.0f/adc[3] * 100.0f;
	 xianshicanshu[3]=2450.0f/adc[5] * 100.0f;
    OLED_Print_Num(0,0,xianshicanshu[0]);
    OLED_Print_Num(0,2,xianshicanshu[1]);//����
    OLED_Print_Num(0,4,xianshicanshu[2]);
    OLED_Print_Num(0,6,xianshicanshu[3]);
    //OLED_Print_Num(56,0,xianshicanshu[3]);
    //OLED_Print_Num(56,2,xianshicanshu[5]);
    //OLED_Print_Num(56,4,xianshicanshu[6]);
    //OLED_Print_Num(56,6,xianshicanshu[7]);
	
	if(5==read_key()) break; //������������������¸�����
    systick_delay_ms(50);
  
  }
}
//��Ӳ˵�
void Add_Menu(uint8 *menu,int16 step)
{
   static uint8 num=0;
   if(num==0) {strcopy(SaveMenu[num],"0.go!!!!");SaveStep[num]=1;num++;}
   strcopy(SaveMenu[num],menu);
   SaveStep[num]=step;
   num++;
}

//�ڶ����˵�
void Next_Menu(uint8 pos)
{
    uint8 i=0;
    char ch[10];
    OLED_Fill(0);
    for(i=pos;i<pos+8;i++)
    {
      if(SaveMenu[i][0]==0) break;                  //ȫ����ʾ��ɣ���ֹ
      else 
      {
        OLED_P6x8Str(15,(i-pos),SaveMenu[i]);//��ʾ��������
        if(i==0) 
        {
          if(conf[SpeedSelect][i]>1)       conf[SpeedSelect][i]=1;
          else if(conf[SpeedSelect][i]<0) conf[SpeedSelect][i]=0;
          if(conf[SpeedSelect][i]) OLED_P6x8Str(92,(i-pos),"SD");
          else OLED_P6x8Str(92,(i-pos),"NORM"); 
        }
        else 
        {
          sprintf(ch, "%d", conf[SpeedSelect][i]);       
          OLED_P6x8Str(92,(i-pos),ch);//��ʾ��������
        }
      }
    }
}   

int16 SpdL=0,SpdR=0;
void precursors_logo(void)
{
  unsigned char x=20,y=0;//x,yλ�ñ���
  unsigned char ch[]={"�Ͼ�ʦ����ѧ"};  
  uint8    NextFlag=0;
  while(NextFlag==0)
  {
    SpdL=Speed_Get(0);//��ȡ������
    SpdR=Speed_Get(1);//��ȡ������
    if(SpdL<0) SpdL=-SpdL;
	if(SpdR<0) SpdR=-SpdR;
	
	GyroX=GetData(GYRO_XOUT_H);//��ȡRoll��ˮƽ����)�����ǽ��ٶ��ź�
    GyroY=GetData(GYRO_YOUT_H);//��ȡPitch(��ֱ����)�����ǽ��ٶ��ź�
    if(GyroX<32768) GyroX+=65536;//���ݴ���
    if(GyroY<32768) GyroY+=65536;//���ݴ���
	GyroX-=62000;//��Сֵ������OLED��ʾ
    GyroY-=62000;//��Сֵ������OLED��ʾ
    x=10;
    y=0;
    OLED_P14x16Str(x,y,ch); 
    x=0;
	y=7;
    OLED_P6x8Str( x, y,"SpdL");
    x+=24;
    OLED_Print_Num1( x, y,SpdL);
    x+=40;
    OLED_P6x8Str( x, y,"SpdR");
    x+=24;
    OLED_Print_Num1( x, y,SpdR);
	x=0;
	y=5;
	OLED_P6x8Str( x, y,"GyrY");
	x=24;	                    
	OLED_Print_Num1( x, y,GyroY);
	x=50;
	y=2;
	OLED_P8x16Str(x,y,"SJC"); 
    for(int16 j=0;j<35;j++)
    {
      if(5==read_key()) {NextFlag=1;break;} //����������¸�����
      systick_delay_ms(1);
    }
  }
}

//�ַ�����������
void strcopy(uint8 *str1,uint8 *str2)
{
  int16 i;
  for(i=0;i<30;i++)
  {
    str1[i]=str2[i];
    if(str2[i]=='\0') break;
  }
}