/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    :本例程中，ESP8266模块工作在AP模式，类似于路由器，可发出wifi热点，手机等可连接
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 调试方式：J-Link-OB
**********************************************************************************/	
//头文件
#include "stm32f10x.h"
#include "esp8266.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"   
#include "led.h" 
#include "OLED.h"
#include "fonts.h"
#include "exti.h"
#include "key.h"
#include "buzz.h"
#include "queue.h"




//函数声明
void Delay_ms( uint16_t time_ms );

u8 *SendData="\r\n我是TCP主机，数据已收到\r\n";
u8  Data_buff[2048];     //数据缓冲区
u8  Data_len[5];         //数据长度缓冲区
u8  Data_id[5];          //数据发送者的id缓冲区

u8 Return_Data[2];//返回给从机的数据

/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
 int main(void)
{
  u8 res;
	Beep_flag=0;
  /*初始化LED端口*/
  LED_GPIO_Config();
	BUZZ_GPIO_Config();
  SYSTICK_Init();
  Queue_Data_Init();
	
	Exti_Config(1);//按键中断配置
  Nvic_Config();//???????按键中断向量配置
	
	OLED_Init(); 
	OLED_DispStr(0, 0,  "Master:WaitConnect", &tFont12);
	OLED_DispStr(0,  15, "------------------", &tFont12);
	OLED_DispStr(0,  30, "SSID:QueuingMaster", &tFont12);
	OLED_DispStr(0,  45, "Password:123456789", &tFont12);

  delay_ms(1500);     //等待芯片上电复位成功
  USART1_Config(9600);	//串口初始化
  USART2_Config(115200);	//串口初始化
  printf("\r\nESP8266准备设置TCP服务器!\r\n");
	
  res=ESP8266_AP_TCPServer();
  switch(res)
  {
    case 0 : printf("ESP8266设置TCP服务器成功!\r\n");break;
    case 1 : printf("ESP8266软件复位失败! 准备重启系统\r\n");NVIC_SystemReset();break;
    case 2 : printf("ESP8266设置AP模式失败! 准备重启系统\r\n");NVIC_SystemReset();break;
    case 3 : printf("ESP8266设置AP参数失败!,准备重启系统\r\n");NVIC_SystemReset();break;
    case 4 : printf("ESP8266关闭透传失败!,准备重启系统\r\n");NVIC_SystemReset();break;
    case 5 : printf("ESP8266开启多路连接失败!,准备重启系统\r\n");NVIC_SystemReset();break;
    case 6 : printf("ESP8266设置AP模式IP失败!,准备重启系统\r\n");NVIC_SystemReset();break;
    case 7 : printf("ESP8266开启设立服务器失败!,准备重启系统\r\n");NVIC_SystemReset();break;
  }
  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //清除串口2数据
	
	
	Beep(1000);//开机成功
  while(1)
  {	   
		if(Beep_flag == 1)
		{
			Beep(200);
			Beep_flag=0;
			Queue_Tail_Join();//加入队尾
		}
    switch(ESP8266_Get_LinkSta())
    {
			//OLED_DispStr(0,  45, "                  ", &tFont12);//清空部分显示
			
      case 1 :	
				printf("从机连接\r\n");
				OLED_DispStr(0,  30, "Connected", &tFont12);
			break;
	    case 2 :  
				printf("从机未连接\r\n");
				OLED_DispStr(0,  30, "Disconnected", &tFont12);
			
			break;
				
    }
    if(ESP8266_Get_Data(Data_buff,Data_len,Data_id))
    {
      printf("连接id:%s有%s字节数据到来\r\n",Data_id,Data_len);
      printf("数据:%s\r\n",Data_buff);
			
			if(Data_buff[0]==0X5A && Data_buff[1]==0X01)//请求队首的指令
			{
				Return_Data[0]=0xA5;//返回数据的帧头
				if(Queue_Head_Inquire()==Queue_Tail_Inquire())//如果队首和队尾相等，说明没有客户
					Return_Data[1]=0;//返回0给从机
				else
				{
					Queue_Head_Depart();
					Return_Data[1]=Queue_Head_Inquire();
				}
				ESP8266_SendData(Char_to_Hex(Data_id,strlen((const char *)Data_id)),Return_Data,2,50);//5s的发送超时时间		数据1 长度 数据2 长度 超时时间
			}
			
    }
  }		
}
