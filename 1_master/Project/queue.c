
#include "queue.h"

//队列就是一个先入先出的结构，我们只需要记下队首和队尾的序号即可

Queue_Data_TypeDef Queue_Data;//定义排队数据结构图


void Queue_Data_Init()
{
	Queue_Data.Head=1;
	Queue_Data.Tail=1;
}

void Queue_Head_Depart()//离开队首
{
	Queue_Data.Head++;
}
void Queue_Tail_Join()//加入队尾
{
	Queue_Data.Tail++;
}

u16 Queue_Tail_Inquire()//查询队尾
{
	return Queue_Data.Tail;
}
u16 Queue_Head_Inquire()//查询队首
{
	return Queue_Data.Head;
}

