
#include "queue.h"

//���о���һ�������ȳ��Ľṹ������ֻ��Ҫ���¶��׺Ͷ�β����ż���

Queue_Data_TypeDef Queue_Data;//�����Ŷ����ݽṹͼ


void Queue_Data_Init()
{
	Queue_Data.Head=1;
	Queue_Data.Tail=1;
}

void Queue_Head_Depart()//�뿪����
{
	Queue_Data.Head++;
}
void Queue_Tail_Join()//�����β
{
	Queue_Data.Tail++;
}

u16 Queue_Tail_Inquire()//��ѯ��β
{
	return Queue_Data.Tail;
}
u16 Queue_Head_Inquire()//��ѯ����
{
	return Queue_Data.Head;
}

