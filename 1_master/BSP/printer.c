
//ͷ�ļ�
#include "printer.h"
#include "usart.h"
#include "queue.h"

void Serial_Printer_Init(void)
{
        //���ÿ�����"ESC @"ʵ�������ʼ����
        printf("\033@");  //��ʼ����ӡ��        
}
void Printer_Cmd_Large()
{
	unsigned char cmd[]={0x1B,0x40,0x1b,0x21,0x30};
	USART_puts(cmd,5);
}
void Printer_Cmd_Normal()
{
	unsigned char cmd[]={0x1B,0x40,0x1b,0x21,0x00};
	USART_puts(cmd,5);
}


void Print_Queue_Tittle()//���СƱ����
{
	printf("********************************");
	printf("\r\n      ��ӭʹ�������Ŷ�ϵͳ\r\n");
	printf("********************************");
	printf("\r\n");
}
void Print_Queue_Number()//����ͻ��������
{
	Printer_Cmd_Large();
	printf("\r\n  ��Ķ��к���\r\n       %d\r\n",Queue_Tail_Inquire());
	Printer_Cmd_Normal();
}
void Print_Queue_Length()//������ڿͻ�ǰ������
{
	printf("\r\n ����ǰ�滹��%d��\r\n",(Queue_Length()-1));
	printf("********************************\r\n");
	printf("\r\n\r\n");
}
void Print_Queue_Info()//����Ŷ�СƱ//////////////
{
	Print_Queue_Tittle();//���СƱ����
	Print_Queue_Number();//�����ǰλ��
	Print_Queue_Length();//������鳤��
	
}