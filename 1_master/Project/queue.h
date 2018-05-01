#ifndef __QUEUE_H
#define __QUEUE_H	 

#include "stm32f10x.h"
typedef struct
{      
	uint16_t Head;   			//���ף���Ա�Ӷ��׿�ʼ����
	uint16_t Tail;   			//��β���ŶӴӶ�β���룩
}Queue_Data_TypeDef;


void Queue_Data_Init();//��ʼ����������
void Queue_Head_Depart();//�뿪����
void Queue_Tail_Join();//�����β
u16 Queue_Tail_Inquire();//��ѯ��β
u16 Queue_Head_Inquire();//��ѯ����


#endif



