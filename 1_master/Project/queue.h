#ifndef __QUEUE_H
#define __QUEUE_H	 

#include "stm32f10x.h"
typedef struct
{      
	uint16_t Head;   			//队首（柜员从队首开始处理）
	uint16_t Tail;   			//队尾（排队从队尾加入）
}Queue_Data_TypeDef;


void Queue_Data_Init();//初始化队列数据
void Queue_Head_Depart();//离开队首
void Queue_Tail_Join();//加入队尾
u16 Queue_Tail_Inquire();//查询队尾
u16 Queue_Head_Inquire();//查询队首


#endif



