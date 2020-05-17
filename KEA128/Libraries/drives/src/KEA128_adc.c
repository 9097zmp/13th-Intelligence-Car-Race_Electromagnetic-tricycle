#include "KEA128_adc.h"
#include "math.h"
#include "KEA128_ftm.h"
#include "init.h"
#include "isr.h"
#define RollPARA         8.0f 
float g_fDirectionError[4];		//����ƫ��
float g_fDirectionError_dot[4];//����ƫ��΢��	
int16 g_ValueOfAD[7]={0};		//��ȡ�ĵ��ֵ
int16 ADOld[5]={0};		//��ȡ�ĵ��ֵ
int16 g_ValueOfADFilter[6]={0};	//�����˲��ĵ��ֵ��δʹ�ã�
int16 XieLv[5]={0};
float kp=0;
float Xiekp=0;
float kp1=0;
float kd=0;
int kpcha=0;//���������Ǽ���kp
int kpcha2=0;//ͣ����־λ
float g_fDirectionError_dotfabs[2]=0;
int16 SpdError[8]={0};
int AnnularFlag=0;
int32 cnt1=0;
int32 cnt2=0;
int32 cnt3=0;
int32 cntout1=0;
int32 cntout2=0;
int32 cntout3=0;
int32 cntout4=0;
int   ringflag=0;
int32 njnutim1=0;
int32 njnutim2=0;
int16 KFC=0;
int16 ttttim=0;
int16 tttsim=0;
int16 tttbim=0;
int32   angle4ms=0;//ˮƽ������ٶ�
int32   angle4mss=0;//��ֱ������ٶ�
int64   anglealltime=0;//ˮƽ����Ƕ�
int64   anglealltimeV=0;//��ֱ����Ƕ�
int8 aa=0;
int8 bb=0;
int16 zhidaotime=0;
int shiziflag=0;
int zhidaoflag=0;
int AnnularWhat=0;//���ƻ�
int AnnularCnt=0;
int anncnt[10]={0};
int16 GyroXLast=0;
int16 admiddle=0;
int   huansize=0;
int32 AnnLet=0;
int32 AnnRit=0;
int32 Annmb=0;
int16 XieKp=0;//б��б���
int16 MidKp=0;
int16 AdVKp=0;
int ganhuangguan[5]={1};
int icx=0;
int shizibug=0;
void ErrorGet(void)
{
	static float g_fDirectionErrorTemp[4][5];
	static float RollSetDiff=0;

	
	ADOld[0] = admiddle ;
	ADOld[1] = g_ValueOfAD[0];
	ADOld[2] = g_ValueOfAD[1];
	Read_ADC();		//��ȡ���ֵ
	g_ValueOfAD[0] = (g_ValueOfAD[0] < 10? 10:g_ValueOfAD[0]);	//6�����ֵ�޷�
	g_ValueOfAD[1] = (g_ValueOfAD[1] < 10? 10:g_ValueOfAD[1]);
	g_ValueOfAD[2] = (g_ValueOfAD[2] < 10? 10:g_ValueOfAD[2]);
	g_ValueOfAD[3] = (g_ValueOfAD[3] < 10? 10:g_ValueOfAD[3]);
	g_ValueOfAD[4] = (g_ValueOfAD[4] < 10? 10:g_ValueOfAD[4]);
	g_ValueOfAD[5] = (g_ValueOfAD[5] < 10? 10:g_ValueOfAD[5]);
	   
	
	g_ValueOfAD[6] = adc_once(ADC0_SE7,ADC_12bit)/100.0f*MidKp;//�޷�2000����м���ֵ
	admiddle = g_ValueOfAD[6];//�м���ֵ
	if(g_ValueOfAD[6]>2000)g_ValueOfAD[6]=2000;
	
	XieLv[0] = admiddle - ADOld[0] ;
	XieLv[1] = g_ValueOfAD[0] - ADOld[1] ;
	XieLv[2] = g_ValueOfAD[0] - ADOld[2] ;
	
	g_fDirectionError[0] = (float)(g_ValueOfAD[0] - g_ValueOfAD[1])/(g_ValueOfAD[0] + g_ValueOfAD[1]);//ˮƽ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[0] = (g_fDirectionError[0]>= 1? 1:g_fDirectionError[0]);	//ƫ���޷�
	g_fDirectionError[0] = (g_fDirectionError[0]<=-1?-1:g_fDirectionError[0]);
	
	
	g_fDirectionError[1] = (float)(g_ValueOfAD[2] - g_ValueOfAD[3])/(g_ValueOfAD[2] + g_ValueOfAD[3]);//��ֱ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[1] = (g_fDirectionError[1]>= 1? 1:g_fDirectionError[1]);	//ƫ���޷�
	g_fDirectionError[1] = (g_fDirectionError[1]<=-1?-1:g_fDirectionError[1]);
	
	g_fDirectionError[2] = (float)(g_ValueOfAD[2] - g_ValueOfAD[3])/(g_ValueOfAD[0] + g_ValueOfAD[1]);//��ֱˮƽ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[2] = (g_fDirectionError[2]>= 1? 1:g_fDirectionError[2]);	//ƫ���޷�
	g_fDirectionError[2] = (g_fDirectionError[2]<=-1?-1:g_fDirectionError[2]);
	
	g_fDirectionError[3] = (float)(g_ValueOfAD[4] - g_ValueOfAD[5])/(g_ValueOfAD[4] + g_ValueOfAD[5]);//����ˮƽ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[3] = (g_fDirectionError[3]>= 1? 1:g_fDirectionError[3]);	//ƫ���޷�
	g_fDirectionError[3] = (g_fDirectionError[3]<=-1?-1:g_fDirectionError[3]);
	  
	g_fDirectionErrorTemp[0][4] = g_fDirectionErrorTemp[0][3];
	g_fDirectionErrorTemp[0][3] = g_fDirectionErrorTemp[0][2];
	g_fDirectionErrorTemp[0][2] = g_fDirectionErrorTemp[0][1];
	g_fDirectionErrorTemp[0][1] = g_fDirectionErrorTemp[0][0];
	g_fDirectionErrorTemp[0][0] = g_fDirectionError[0];
	g_fDirectionError_dot[0] = 5*(g_fDirectionErrorTemp[0][0]-g_fDirectionErrorTemp[0][3]);//ˮƽ��е�ƫ��΢��
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]> 0.7? 0.7:g_fDirectionError_dot[0]);//ƫ��΢���޷�
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]<-0.7?-0.7:g_fDirectionError_dot[0]);

	g_fDirectionError_dotfabs[0] = 5*(fabs(g_fDirectionErrorTemp[0][0])-fabs(g_fDirectionErrorTemp[0][3]));

	g_fDirectionErrorTemp[1][4] = g_fDirectionErrorTemp[1][3];
	g_fDirectionErrorTemp[1][3] = g_fDirectionErrorTemp[1][2];
	g_fDirectionErrorTemp[1][2] = g_fDirectionErrorTemp[1][1];
	g_fDirectionErrorTemp[1][1] = g_fDirectionErrorTemp[1][0];
	g_fDirectionErrorTemp[1][0] = g_fDirectionError[1];
	g_fDirectionError_dot[1] = 5*(g_fDirectionErrorTemp[1][0]-g_fDirectionErrorTemp[1][3]);//��ֱ��е�ƫ��΢��
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]> 0.7? 0.7:g_fDirectionError_dot[1]);//ƫ��΢���޷�
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]<-0.7?-0.7:g_fDirectionError_dot[1]);
	
	g_fDirectionErrorTemp[2][4] = g_fDirectionErrorTemp[2][3];
	g_fDirectionErrorTemp[2][3] = g_fDirectionErrorTemp[2][2];
	g_fDirectionErrorTemp[2][2] = g_fDirectionErrorTemp[2][1];
	
	g_fDirectionErrorTemp[2][1] = g_fDirectionErrorTemp[2][0];
	g_fDirectionErrorTemp[2][0] = g_fDirectionError[2];
	g_fDirectionError_dot[2] = 5*(g_fDirectionErrorTemp[2][0]-g_fDirectionErrorTemp[2][3]);//��ֱ��е�ƫ��΢��
	g_fDirectionError_dot[2] = (g_fDirectionError_dot[2]> 0.7? 0.7:g_fDirectionError_dot[2]);//ƫ��΢���޷�
	g_fDirectionError_dot[2] = (g_fDirectionError_dot[2]<-0.7?-0.7:g_fDirectionError_dot[2]);
	
	g_fDirectionErrorTemp[3][4] = g_fDirectionErrorTemp[3][3];
	g_fDirectionErrorTemp[3][3] = g_fDirectionErrorTemp[3][2];
	g_fDirectionErrorTemp[3][2] = g_fDirectionErrorTemp[3][1];
	g_fDirectionErrorTemp[3][1] = g_fDirectionErrorTemp[3][0];
	g_fDirectionErrorTemp[3][0] = g_fDirectionError[3];
	g_fDirectionError_dot[3] = 5*(g_fDirectionErrorTemp[3][0]-g_fDirectionErrorTemp[3][3]);//��ֱ��е�ƫ��΢��
	g_fDirectionError_dot[3] = (g_fDirectionError_dot[3]> 0.7? 0.7:g_fDirectionError_dot[3]);//ƫ��΢���޷�
	g_fDirectionError_dot[3] = (g_fDirectionError_dot[3]<-0.7?-0.7:g_fDirectionError_dot[3]);
			
///////////*****************������******************//////////////////	
     GyroXLast=GyroX;
	 GyroXo=GyroX;  
     GyroX=GetData(GYRO_XOUT_H);
     if(GyroX<32768) GyroX+=65535;
     GyroX=GyroX-18+5;
     if((GyroX-GyroXo)>32768) GyroX-=65535;
     else if((GyroXo-GyroX)>32768) GyroX+=65535; 
	
	if(timet>=375)//����1������µ�
	{
	 
	 GyroYo=GyroY;  
     GyroY=GetData(GYRO_YOUT_H);
     if(GyroY<32768) GyroY+=65535;
     GyroY=GyroY-11;
     if((GyroY-GyroYo)>32768) GyroY-=65535;
     else if((GyroYo-GyroY)>32768) GyroY+=65535;
	 
	  angle4mss=GyroY/100;
	 anglealltimeV+=angle4mss;
	 
	 
	 if(admiddle>2400)
		 DianBoFlag=5;
	 if(DianBoFlag==5)
		 DianBoTim++;
	 if(DianBoTim>33)
	 {
		 DianBoTim=0;
	     DianBoFlag=0;
	 }
	 
	 
		 if(fabs(GyroY)>3000&&PoFlag==0)
		 {
		  if(DianBoFlag==0)
			  PoFlag=1;//����
		  else if(DianBoFlag==5)
			  PoFlag=2;//�µ�
	   
		 }
	   if(PoFlag==1||PoFlag==2)
	   {
		   PoTim++;
	   }
	   if(PoTim>=njnutim2)//�µ����һ������ʱ
	   {
		   PoTim=0;
		   PoFlag=0;
	   }
	 	 
	}
	
///////////*****************������******************//////////////////
//////////**************ʮ��***********/////////////	
	if(g_ValueOfAD[3]>1600&&g_ValueOfAD[2]>1600)
		shiziflag=1;
	if(shiziflag==1)
		cnt3++;
	if(cnt3>80)
	{cnt3=0;shiziflag=0;}
//////////**************ʮ��***********/////////////
	
//////////**************ֱ��***********/////////////		
	if(GyroX<=1500&&GyroX>=-1500)
	zhidaotime++;
	if(zhidaotime>20)
		zhidaoflag=1;
	if(GyroX>1500||GyroX<-1500)
	{
		zhidaotime=0;
		zhidaoflag=0;
	}
	
	/*	if(GyroX>1500&&InAnn==0)
			njnutim1++;
	if(njnutim1>125)
		InAnn=1;
	if(GyroX<500||InAnn==1)
		    njnutim1=0;*/
		   
		

//////////**************ֱ��***********/////////////

    kp1 = kp + 18 * (1000-fabs(GyroX))/1000.0f;
	Xiekp = 55 + 12 * (1000-fabs(GyroX))/1000.0f;

	if((AnnularWhat==1||AnnularFlag>0)&&(huansize==0||huansize==1||huansize==5||huansize==6))
	{
		kp1=kp;
	}
	
	SpdError[0] = g_fDirectionError[0] * kp1 + g_fDirectionError_dot[0] * kd;//���Ⱥ�
		
	SpdError[1] = g_fDirectionError[1] * 160 + g_fDirectionError_dot[1] * 140;//����Ⱥ�
	
	SpdError[2] = g_fDirectionError[2] * kp1 + g_fDirectionError_dot[2] * kd;//������Ⱥ�
	
	if(g_ValueOfAD[3]>1600&&g_ValueOfAD[2]>1600)//ʮ��

	SpdError[2]=0;
	
	SpdError[3] = g_fDirectionError[3]*Xiekp + g_fDirectionError_dot[3]*55;//б��Ⱥͱ�Kp
	
	SpdError[4] = g_fDirectionError[0]*140 + g_fDirectionError_dot[0]*180;
	
	SpdError[5] = g_fDirectionError[3]*55+ g_fDirectionError_dot[3]*55;//б��ȺͶ�Kp
	
    //SpdError[6] = (g_fDirectionError[0]>0? 2000-g_ValueOfAD[6]:g_ValueOfAD[6]-2000)/(ADOld[0]*2 )*moluntai;
	
	AnnJudge();//����ʶ��

	SpdError[7] = 10*1.0f/10*SpdError[0]+23*1.0f/10*SpdError[2];//����ƫ��
	
	huansize = anncnt[AnnularCnt%4];//������С����
	
	if(PoFlag>0&&PoTim<180)//�µ�0.7��ƫ��
		
	SpdError[7] = SpdError[0];
	   
	if(AnnularWhat==1)//Ԥ�컷ƫ��
	{
		if(huansize==1||huansize==5)
			SpdError[7] = 0*SpdError[3] + 2*SpdError[0] + 0.0f*(huansize>3?-3:3)*(-120+(Speednow<120?120:Speednow))/180.0f;
	    else if(huansize==0||huansize==6)
			SpdError[7] = 0*SpdError[3] + 3*SpdError[0] + 0.0f*(huansize>3?-3:3)*(-120+(Speednow<120?120:Speednow))/180.0f;
		else if(huansize==2)
			SpdError[7] = 0.9*SpdError[5] + 0.9*SpdError[0]+(20.0f*(huansize>3?-3:3)-(huansize-4)*AnnLet-(huansize-2)*AnnRit)*(-120+(Speednow<120?120:Speednow))/180.0f;
	    else if(huansize==4)
			//SpdError[7] = 0.9*SpdError[5] + 0.9*SpdError[0]+(30.0f*(huansize>3?-3:3)-(huansize-4)*AnnLet-(huansize-2)*AnnRit)*(-120+(Speednow<120?120:Speednow))/180.0f;
	         SpdError[7] = 0.9*SpdError[5] + 0.0*SpdError[0]+0*(20.0f*(huansize>3?-3:3)-(huansize-4)*AnnLet-(huansize-2)*AnnRit)*(-120+(Speednow<120?120:Speednow))/180.0f;
	}
	if(AnnularFlag==1)//��ǽ���
	{
		if(huansize==1||huansize==5)
			SpdError[7] = 0.9*SpdError[3] + 0.9*SpdError[0] + 36.0f*(huansize>3?-3:3)*(-120+(Speednow<120?120:Speednow))/180.0f;
		else if(huansize==0||huansize==6)
			SpdError[7] = 0.9*SpdError[3] + 0.9*SpdError[0] + Annmb*(huansize>3?-3:3)*(-120+(Speednow<120?120:Speednow))/180.0f;
		else if(huansize==2)
			SpdError[7] =((10*1.0f/10*SpdError[0]+23*1.0f/10*SpdError[2])*1.1-(huansize-4)*AnnLet-(huansize-2)*AnnRit)*(-100+(Speednow<120?120:Speednow))/180.0f;
	    else if(huansize==4)
			//SpdError[7] =((10*1.0f/10*SpdError[0]+23*1.0f/10*SpdError[2])*1.1-(huansize-4)*AnnLet-(huansize-2)*AnnRit)*(-90+(Speednow<120?120:Speednow))/180.0f;
	        SpdError[7] = 0.9*SpdError[3] + 0.9*SpdError[0] + AnnRit*(huansize>3?-2:2)*(-120+(Speednow<120?120:Speednow))/180.0f;
	}
	
	if(AnnularFlag==2)//����
	{   
		if(huansize==0||huansize==1||huansize==5||huansize==6)
		SpdError[7] = (SpdError[3] + 0.9*SpdError[0]);
		else if(huansize==4)
		{
		if(Speednow<200)
		SpdError[7] = (10*1.0f/10*SpdError[0]+13*1.0f/10*SpdError[2])*(-100+(Speednow<120?120:Speednow))/220.0f;
		else
		SpdError[7] = (10*1.0f/10*SpdError[0]+20*1.0f/10*SpdError[2])*(-100+(Speednow<120?120:Speednow))/120.0f;//С����
		}
		else
		{
		if(Speednow<200)
		SpdError[7] = (10*1.0f/10*SpdError[0]+4*1.0f/10*SpdError[2])*(-100+(Speednow<120?120:Speednow))/220.0f;
		else
		SpdError[7] = (10*1.0f/10*SpdError[0]+20*1.0f/10*SpdError[2])*(-100+(Speednow<120?120:Speednow))/120.0f;//С����
		}	
	}
	
    
	if(AnnularFlag==2&&fabs(anglealltime)>10000&&(huansize==0||huansize==6))//�󻷳���ǰ
		    SpdError[7] = 2*SpdError[5] + 0.9*SpdError[0];
	else if(AnnularFlag==2&&fabs(anglealltime)>9700&&(huansize==1||huansize==5))//�л�����ǰ
		    SpdError[7] = 1.5*SpdError[5] + 0.93*SpdError[0];
	else if(huansize==2&&AnnularFlag==2&&fabs(anglealltime)>10000)//С�������ǰ
		    SpdError[7]=SpdError[0]+14/10.0f * SpdError[5];
	else if(huansize==4&&AnnularFlag==2&&fabs(anglealltime)>9700)//С���ҳ���ǰ
	        SpdError[7]=SpdError[0]+14/10.0f * SpdError[5];
		
	if(AnnularFlag==3)//����ʱ
	{
		if(huansize==2||huansize==4)
            SpdError[7]=(SpdError[4]-(0.5-0.5*cntout3/ttttim)*SpdError[1])*0.7f;	
		else if(huansize==1||huansize==5)
			SpdError[7]=(SpdError[4]-(0.6-0.4*cntout3/ttttim)*SpdError[1])*0.7f;
		else
			SpdError[7]=(SpdError[4]-(0.6-0.35*cntout3/tttbim)*SpdError[1])*0.7f;
	}
	if(timet>(Endtim*2.5f))
	{
	for(icx=0;icx<3;icx++)
	{	
		ganhuangguan[icx]=gpio_get(E4);
	}
	}

///////////*****************������**************////////////
	if(AnnularFlag>0&&AnnularFlag<4)
       ringflag=1;
	/*else if(AnnularWhat==1)
	   ringflag=1;*/
	else if(timet<StartTime)
	   ringflag=1;
	else if(PoFlag==1&&PoTim<250)
	   ringflag=2;
	else if(PoFlag==2&&PoTim<250)
	   ringflag=3;
	/*else if(zhidaoflag==1)
		ringflag=1;*/
	/*if(Speednow<200)
		ringflag=1;
	
	/*else if((g_ValueOfAD[3]>1600&&g_ValueOfAD[2]>1600))
		ringflag=1;*/
	else ringflag=0;
	
	/*if(StpFlag==1)
	ringflag=1;*/

    ring_control();
///////////*****************������******************////////////

	
//*/*/*/*/*/*/*/********ƫ���޷�************/*/*/*/*/*/*/*/*//
	if(SpdError[7]>600)SpdError[7]=600;
	if(SpdError[7]<-600)SpdError[7]=-600;
	SpdError[7]=-10*SpdError[7];
//*/*/*/*/*/*/*/********ƫ���޷�************/*/*/*/*/*/*/*/*//
}

void AnnJudge(void)
{	
	if(huansize==2||huansize==4)
	{
	if((((g_ValueOfAD[0]>1100&&g_ValueOfAD[1]>1100)||(g_ValueOfAD[0]+g_ValueOfAD[1]>3200))&&((g_ValueOfAD[2]+g_ValueOfAD[3])<2000)))	//1200 1200 3500 1600 
    AnnularWhat=1;
    else 
    AnnularWhat=0;
	}
	else 
	{
	if((((g_ValueOfAD[0]>1300&&g_ValueOfAD[1]>1300)||(g_ValueOfAD[0]+g_ValueOfAD[1]>3600))&&((g_ValueOfAD[2]+g_ValueOfAD[3])<5000)))	//1200 1200 3500 1600 
    AnnularWhat=1;
    else 
    AnnularWhat=0;
	}	

	if(shizibug==1)
	{
		if(PoFlag>0&&PoTim<200)
		AnnularWhat=0; 
	}


if(huansize==0||huansize==1||huansize==5||huansize==6)
{
	if(((g_ValueOfAD[0]+g_ValueOfAD[1])>2600)&&AnnularFlag==0&&g_ValueOfAD[2]<800&&g_ValueOfAD[3]<800&&(g_ValueOfAD[4]>1500||g_ValueOfAD[5]>1500||g_ValueOfAD[0]>3000))
{
	AnnularFlag=1;
}
}
else
{
	if(((g_ValueOfAD[0]+g_ValueOfAD[1])>2600)&&AnnularFlag==0&&(g_ValueOfAD[4]>1800||g_ValueOfAD[5]>1800||g_ValueOfAD[0]>3000)&&(g_ValueOfAD[2]<600||g_ValueOfAD[3]<600))
{
	AnnularFlag=1;
}
	
	
}
if(AnnularFlag==1&&((g_ValueOfAD[0]+g_ValueOfAD[1])<2800))
AnnularFlag=2;

if(AnnularFlag==2)
 cntout1++;
if(cntout1>300)cntout1=300;
if(AnnularFlag==0)
 cntout1=0;

if(AnnularFlag==1||AnnularFlag==2)
{ 
     angle4ms=GyroX/100;
	 anglealltime+=angle4ms;
}
if(huansize==0||huansize==6)
{	
if(AnnularFlag==2&&(fabs(anglealltime)>13000))//14000
{
	AnnularFlag=3;
}
}
else if(huansize==1||huansize==5)
{	
if(AnnularFlag==2&&(fabs(anglealltime)>12800))//14000
{
	AnnularFlag=3;
}
}
else if(huansize==4)
{
if(AnnularFlag==2&&(fabs(anglealltime)>(12500)))//zuo12500
{
	AnnularFlag=3;
}
}
else
{
if(AnnularFlag==2&&(fabs(anglealltime)>12500))//zuo12500
{
	AnnularFlag=3;
}
}

if(AnnularFlag==3)

{ 
     angle4ms=0;
	 anglealltime=0;
}

if(AnnularFlag==3)
     cntout3++;
if(huansize==1||huansize==5)
{
     if(cntout3>ttttim)
     {
         cntout3=0;
	     AnnularFlag=0;
		 AnnularCnt++;
     }
}
else if(huansize==0||huansize==6)
{
     if(cntout3>tttbim)
     {
         cntout3=0;
	     AnnularFlag=0;
		 AnnularCnt++;
     }
}
else
{
	if(cntout3>tttsim)
     {
         cntout3=0;
	     AnnularFlag=0;
		 AnnularCnt++;
     }
}
	//if(PoFlag==1&&AnnularFlag>0&&AnnularFlag<3)
    /*if(PoHuan==1)
	{
		if(PoFlag==1)
		{
			if(huansize==2||huansize==4)
			{
				if(((g_ValueOfAD[0]+g_ValueOfAD[1])>2600)&&AnnularFlag==0&&(g_ValueOfAD[4]>1900||g_ValueOfAD[5]>1900||admiddle>3000)&&(g_ValueOfAD[2]<600||g_ValueOfAD[3]<600))
			    AnnularFlag==0;
			}
		}
	}*/
	 if(PoHuan==1)
	 {
	 if(AnnularFlag==2)//������
	 {
		 if((huansize<3&&angle4ms>0)||(huansize>3&&angle4ms<0))
		 {
			 AnnularFlag=0;
			 cntout1=0;
			 angle4ms=0;
			 anglealltime=0;
		 }
	 }
	 }
		

}





void Read_ADC(void)
{
     int16  i,j,k,temp;
     int16  ad_valu[6][5],ad_valu1[6],ad_sum[6];
	 int16 ValueOfADOld[6],ValueOfADNew[6];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=ad_ave(AD1, ADC_12bit, 5);// AD1ˮƽ��
         ad_valu[1][i]=ad_ave(AD2, ADC_12bit, 5);// AD2ˮƽ��
         ad_valu[2][i]=ad_ave(AD3, ADC_12bit, 5)/100.0f*AdVKp;// AD3��ֱ��
         ad_valu[3][i]=ad_ave(AD4, ADC_12bit, 5)/100.0f*AdVKp;// AD4��ֱ��
		 ad_valu[4][i]=ad_ave(AD5, ADC_12bit, 5)/100.0f*XieKp;// AD4��ֱ��
		 ad_valu[5][i]=ad_ave(AD6, ADC_12bit, 5)/100.0f*XieKp;// AD4��ֱ��
     }
/*=========================ð����������==========================*///�������ֵ����Сֵ
     for(i=0;i<6;i++)
     {
        for(j=0;j<4;j++)
        {
           for(k=0;k<4-j;k++)
           {
              if(ad_valu[i][k] > ad_valu[i][k+1])
              {
                 temp = ad_valu[i][k+1];
                 ad_valu[i][k+1] = ad_valu[i][k];
                 ad_valu[i][k] = temp;
              }
           }
        }
     }
/*===========================��ֵ�˲�=================================*/
     for(i=0;i<6;i++)    //���м�����ĺ�
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
     }

	
	 for(i=0;i<6;i++)            //����ֵ�и�λ������
	 {
	 	g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);

		//�ɼ��ݶ�ƽ����ÿ�βɼ����仯40
		ValueOfADOld[i] = g_ValueOfADFilter[i];
		ValueOfADNew[i] = g_ValueOfAD[i];
		
		if(ValueOfADNew[i]>=ValueOfADOld[i])
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);
		else
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);
	 }	 
}


uint16 ad_ave(ADCn_Ch adcn_ch, ADC_nbit bit,uint8 N) //��ֵ�˲�
{
    uint32 tmp = 0;
    uint8  iccd;
    uint8  jccd;

    for(iccd = 0; iccd < N; iccd++)
	{
        tmp += adc_once(adcn_ch, bit);
		jccd = 20;
		while(jccd--);
	
	}
    tmp = tmp / N;
    return (uint16)tmp;
}


__ramfunc void ring_control(void)
{
    static int ringcount=0;
    if(ringflag==0)
    {
        ring_off();
        ringcount=0;
    }
    else if(ringflag==1)
    {
        ring_on();
        ringcount=0;
    }
    else if(ringflag==2)
    {
        ringcount++;
        if(ringcount<=25)   ring_on();
        else                ring_off();
        if(ringcount>=50)  ringcount=0;
    }
    else if(ringflag==3)
    {
        ringcount++;
        if(ringcount<=50)  ring_on();
        else                ring_off();
        if(ringcount>=100)  ringcount=0;
    }
    else if(ringflag==4)
    {
        ringcount++;
        if(ringcount<=50)  ring_on();
        else                ring_off();
        if(ringcount>=250)  ringcount=0;
    }
}


void adc_init(ADCn_Ch adcn_ch)
{
    SIM->SCGC |= SIM_SCGC_ADC_MASK;         //����ADCʱ��
    
    ADC->APCTL1 |= 1<<adcn_ch;              //ʹ��ADC����
    
    ADC->SC2 = ADC_SC2_REFSEL(1);           //��׼��ѹѡ��
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ�񾫶�ADC_8bit��ADC_10bit��ADC_12bit
//  @return     void
//  @since      v2.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8,ADC_12bit);//�ɼ�ADC0_SE8��ѹ������12λ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_once(ADCn_Ch adcn_ch, ADC_nbit bit)
{
    if(1 == KEA128_CLOCK)
    {
        //��Ƶ������ADC��CLKΪ����ʱ��/2
        ADC->SC3 = (0
                    | ADC_SC3_ADIV(1)           //��Ƶϵ��
                    | ADC_SC3_MODE(bit)         //�ֱ���
                    | ADC_SC3_ADICLK(0)         //ʹ������ʱ����ΪADC��ʱ��Դ
                    //| ADC_SC3_ADLSMP_MASK       //1��������ʱ��  0���̲���ʱ��   ע��Ϊ0 ������ʱ��ɼ����ȶ�
                    );
    }
    else
    {
        //δ��Ƶ������ADC��CLKΪ����ʱ��
        ADC->SC3 = (0
                    | ADC_SC3_ADIV(0)           //��Ƶϵ��
                    | ADC_SC3_MODE(bit)
						//�ֱ���
                    | ADC_SC3_ADICLK(0)         //ʹ������ʱ����ΪADC��ʱ��Դ
                    //| ADC_SC3_ADLSMP_MASK       //1��������ʱ��  0���̲���ʱ��   ע��Ϊ0 ������ʱ��ɼ����ȶ�
                    );
    }
    

    ADC->SC1 = ADC_SC1_ADCH(adcn_ch);       //����ת��
    
    while(!(ADC->SC1 & ADC_SC1_COCO_MASK)); //�ȴ�ת�����
    return (ADC->R & ADC_R_ADR_MASK);       //���ؽ��
    
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCֹͣ�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ�񾫶�ADC_8bit��ADC_10bit��ADC_12bit
//  @return     void
//  @since      v2.0
//  Sample usage:               adc_stop(adc0);
//-------------------------------------------------------------------------------------------------------------------
void adc_stop(void)
{
    ADC->SC1 = ADC_SC1_ADCH(0xFF); 
}
