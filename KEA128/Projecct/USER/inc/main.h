#ifndef __MAIN_H   
#define __MAIN_H 
#include "common.h"
#define ConfigNum       35            //���ɵ���������
#define FlashSector     255
#define Gyrorate        0.1775f 
extern uint8      imgmode;                  //ģʽѡ��
extern uint8      SpeedSelect;              //�ٶ�ѡ��
extern uint32  TimeRunCount;
extern uint16  adc[8];
extern int16   conf[3][ConfigNum];
extern uint8      SDOrNormFlag;
extern int32   GyroX,GyroXo,GyroY,GyroYo; 
extern int32   Gyropitchfloat,Gyrorollfloat;                //ֱ�����������,ת�����������
extern int32   Anglefloat;   
extern int32   Acc_Z;
#endif  //__INIT_H