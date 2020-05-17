#include "imgwrite.h"
/******************** (C) COPYRIGHT 2014 njnu Precursors ********************
 * �ļ���       ��imgwrite.c
 * ����         : SD��д����
 *              ��BusClock=120MHZ; 
 * ʵ��ƽ̨     ��Precursors board
*****************************************************************************/
//����д��D_flash
void flashconf_write(void)
{
  int16 i=0;
  FLASH_EraseSector(FlashSector);  //�������һ������
  /*for(i=0;i<ConfigNum;i++)
  {
    FLASH_WriteSector(FlashSector,(const uint8 *)(conf[0][i]),2,2*i); 
  }*/
  FLASH_WriteSector(FlashSector,(const uint8 *)conf[0],ConfigNum*2,0);
  
}

//��D_flash��ȡ����
void flashconf_read(void)
{
  int16 i=0;
  uint32 temp=0;
  for(i=0;i<ConfigNum;i++)
  {
    temp=flash_read(FlashSector,2*i,uint16);
    conf[0][i]=temp;
  }
}
