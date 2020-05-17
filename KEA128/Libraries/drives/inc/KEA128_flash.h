#ifndef _KEA128_flash_h
#define _KEA128_flash_h

#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"

#define FTF    				    FTMRE

#define SECTOR_SIZE     		(512)
#define FLASH_SECTOR_NUM        (256)                   //������
#define FLASH_ALIGN_ADDR        4                       //��ַ����������
typedef uint32                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�ú궨���flash�������ݶ�ȡ
//  @param      SectorNum 		��Ҫд����������
//  @param      offset	 		��ַƫ��
//  @param      type		 	��ȡ����������
//  @return     				���ظ�����ַ������
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//��ȡ20������ƫ��0��������Ϊuint32
//-------------------------------------------------------------------------------------------------------------------
#define     flash_read(SectorNum,offset,type)        (*(type *)((uint32)(((SectorNum)*SECTOR_SIZE) + (offset))))


void FLASH_Init(void);
uint32 FLASH_GetSectorSize(void);
uint8 FLASH_EraseSector(uint32 SectorNum);
uint8 FLASH_WriteSector(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset);



#endif
