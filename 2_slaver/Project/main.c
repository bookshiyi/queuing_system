/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    :本例程中，ESP8266模块工作在STA（客户端）模式，即终端模式。需要连接无线路由器或是其他的AP模式  
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 调试方式：J-Link-OB
**********************************************************************************/	

#include "delay.h"
#include "esp8266.h"
#include "usart.h"
#include "usart2.h"
#include "OLED.h"
#include "fonts.h"
#include "exti.h"
#include "led.h"
#include "key.h"
#include "buzz.h"

u8  Data_buff[1024];     //数据缓冲区
u8  Data_len[5];         //数据长度缓冲区
u8  Data_id[5];          //数据发送者的id缓冲区


u8 Send_Data[2];		//给主机发送数据的缓存

/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{	
  u8 res1,res2,i;
	u8 num[4];
  u16 len;

  SYSTICK_Init();	    //延时函数初始化
  LED_GPIO_Config();
	BUZZ_GPIO_Config();
	Exti_Config(1);//按键中断配置
  Nvic_Config();//???????按键中断向量配置
	
  delay_ms(1500);     //等待芯片上电复位成功
	OLED_Init(); 
  USART1_Config(9600);	//串口初始化
  uart2_init(115200);	//串口初始化
	
	delay_ms(1000);
	printf("<<Z>0"); 
	delay_ms(3000);
	printf("<V>4"); 	//音量设置
	delay_ms(2000);

	

	OLED_DispStr(0, 15, "  Slaver OK  ", &tFont12);
	OLED_DispStr(0, 15, "------------------", &tFont12);
	OLED_DispStr(15, 45, "Please Wait...", &tFont12);     
	

  ESP8266_Exit_Transfer(10);//检测是否在透传状态，如果在则退出
  printf("\r\n连接主机!\r\n");

  res1=ESP8266_STA_TCPCLient();
  switch(res1)
  {
    case 0  :  printf("连接主机成功，进入透传\r\n");
								OLED_DispStr(0, 45, "Connected Success", &tFont12);
								break;
    case 1  :  printf("软件复位失败! 准备重启\r\n");
								OLED_DispStr(0, 45, "S-reset failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 2  :  printf("设置STA模式失败! 准备重启\r\n");
								OLED_DispStr(0, 45, "STA failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 3  :  printf("连接AP失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "C-AP failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 4  :  printf("的IP获取失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "IP failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 5  :  printf("开启透传失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "transmission failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 6  :  printf("关闭多路连接失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "Multiple failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 7  :  printf("连接主机失败，主机未开启，准备重连\r\n");
								OLED_DispStr(0, 45, "Master Closed", &tFont12);
								;break;
    case 8  :  printf("已经连接主机\r\n");
								OLED_DispStr(0, 45, "Connected", &tFont12);
								break;
    case 9  :  printf("连接主机失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "AP-failed failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 10 :  printf("进入透传失败!,准备重启\r\n");
								OLED_DispStr(0, 45, "transmission failed", &tFont12);
								NVIC_SystemReset();
								break;
  }
  if(res1==7)
  {
      res2=1;
      
      while(res2)
      {
        res2=ESP8266_Connect_Server(SERVER_IP,SERVER_PORT,50);     //连接TCP服务器,并进入透传，超时时间5s，失败返回7
        switch(res2)
        {
          case 0 :  printf("进入透传\r\n");break;//连接主机成功
          case 1 :  printf("连接主机失败，主机未开启，准备重连\r\n");break;
          case 2 :  printf("已连接主机\r\n");break;
          case 3 :  printf("连接主机失败!,准备重启\r\n");NVIC_SystemReset();break;
          case 4 :  printf("进入透传失败!,准备重启\r\n");NVIC_SystemReset();break;
        }
        for(i=0;i<20;i++)
        delay_ms(1000);    //延时20s
      }		
  }
  USART2_DATA_FLAG=1;  //标记进入数据透传了
  USART2_RX_STA=0;  
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //清除串口2数据
  while(1)
  {
    if(USART2_RX_STA&0x8000)//如果收到主机发来的数据
    {
      len=USART2_RX_STA&0x7FFF;
      memcpy(Data_buff,USART2_RX_BUF,len);
      USART2_RX_STA=0;		
      Data_buff[len]='\0';
			if(Data_buff[0]==0xA5 && Data_buff[1]>0)
			{
				//Data_buff[1] = Data_buff[1]+'0';
				
				num[0]=Data_buff[1]/100 +48;
				num[1]=Data_buff[1]%100/10 +48;
				num[2]=Data_buff[1]%100%10 +48;
				num[3]=0;
				printf("<Z>1");
				delay_ms(1500);
				printf("<Z>4");
				delay_ms(1500);
				printf("请 %s号客户办理业务\r\n",num);
				
				OLED_DispStr(0, 45, "正在接待  号客户", &tFont12);//无人排队
				OLED_DisDigital(30, 45,Data_buff[1], &tFont12);//oled显示
			}
			else if(Data_buff[1]==0)//暂时无人排队
			{
				printf("暂时没有排队客户\r\n");
				OLED_DispStr(0, 45, "Queue is Empty", &tFont12);//无人排队
			}
			
    }
    if(Beep_flag == 1)
		{
			Beep(200);
			Beep_flag=0;
			Send_Data[0]=0X5A;
			Send_Data[1]=0X01;//请求队首的数据
			
			u2_printf("%s",Send_Data);//数据发送给主机
		}

  }
}

