#include "KEA128_port.h"
#define PTX(PTX_n)  (PTX_n>>5)  //��ȡģ��� PTX_n/32
#define PTn(PTX_n)  (PTX_n&0x1f)//��ȡ���ź� PTX_n%32

//-------------------------------------------------------------------------------------------------------------------
//  @brief      IO����
//  @param      ptx_n           ѡ��˿�
//  @return     void
//  @since      v2.0
//  Sample usage:               port_pull(E0);			//E0 �������迪��
//-------------------------------------------------------------------------------------------------------------------
void port_pull(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    switch(ptx)
    {
        case 0:
        {
            PORT->PUE0 |= (uint32)(1<<ptn);
        }break;
        
        case 1:
        {
            PORT->PUE1 |= (uint32)(1<<ptn);
        }break;
        
        case 2:
        {
            PORT->PUE2 |= (uint32)(1<<ptn);
        }break;
    }
}

