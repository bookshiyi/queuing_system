
//头文件
#include "printer.h"
#include "usart.h"
#include "queue.h"

void Serial_Printer_Init(void)
{
        //利用控制码"ESC @"实现软件初始化。
        printf("\033@");  //初始化打印机        
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


void Print_Queue_Tittle()//输出小票标题
{
	printf("********************************");
	printf("\r\n      欢迎使用银行排队系统\r\n");
	printf("********************************");
	printf("\r\n");
}
void Print_Queue_Number()//输出客户队列序号
{
	Printer_Cmd_Large();
	printf("\r\n  你的队列号码\r\n       %d\r\n",Queue_Tail_Inquire());
	Printer_Cmd_Normal();
}
void Print_Queue_Length()//输出排在客户前的人数
{
	printf("\r\n 队列前面还有%d人\r\n",(Queue_Length()-1));
	printf("********************************\r\n");
	printf("\r\n\r\n");
}
void Print_Queue_Info()//输出排队小票//////////////
{
	Print_Queue_Tittle();//输出小票标题
	Print_Queue_Number();//输出当前位置
	Print_Queue_Length();//输出队伍长度
	
}