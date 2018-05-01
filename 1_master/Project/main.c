/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    :�������У�ESP8266ģ�鹤����APģʽ��������·�������ɷ���wifi�ȵ㣬�ֻ��ȿ�����
 * ��汾  ��V3.50
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	
//ͷ�ļ�
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




//��������
void Delay_ms( uint16_t time_ms );

u8 *SendData="\r\n����TCP�������������յ�\r\n";
u8  Data_buff[2048];     //���ݻ�����
u8  Data_len[5];         //���ݳ��Ȼ�����
u8  Data_id[5];          //���ݷ����ߵ�id������

u8 Return_Data[2];//���ظ��ӻ�������

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
  /*��ʼ��LED�˿�*/
  LED_GPIO_Config();
	BUZZ_GPIO_Config();
  SYSTICK_Init();
  Queue_Data_Init();
	
	Exti_Config(1);//�����ж�����
  Nvic_Config();//???????�����ж���������
	
	OLED_Init(); 
	OLED_DispStr(0, 0,  "Master:WaitConnect", &tFont12);
	OLED_DispStr(0,  15, "------------------", &tFont12);
	OLED_DispStr(0,  30, "SSID:QueuingMaster", &tFont12);
	OLED_DispStr(0,  45, "Password:123456789", &tFont12);

  delay_ms(1500);     //�ȴ�оƬ�ϵ縴λ�ɹ�
  USART1_Config(9600);	//���ڳ�ʼ��
  USART2_Config(115200);	//���ڳ�ʼ��
  printf("\r\nESP8266׼������TCP������!\r\n");
	
  res=ESP8266_AP_TCPServer();
  switch(res)
  {
    case 0 : printf("ESP8266����TCP�������ɹ�!\r\n");break;
    case 1 : printf("ESP8266�����λʧ��! ׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 2 : printf("ESP8266����APģʽʧ��! ׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 3 : printf("ESP8266����AP����ʧ��!,׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 4 : printf("ESP8266�ر�͸��ʧ��!,׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 5 : printf("ESP8266������·����ʧ��!,׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 6 : printf("ESP8266����APģʽIPʧ��!,׼������ϵͳ\r\n");NVIC_SystemReset();break;
    case 7 : printf("ESP8266��������������ʧ��!,׼������ϵͳ\r\n");NVIC_SystemReset();break;
  }
  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //�������2����
	
	
	Beep(1000);//�����ɹ�
  while(1)
  {	   
		if(Beep_flag == 1)
		{
			Beep(200);
			Beep_flag=0;
			Queue_Tail_Join();//�����β
		}
    switch(ESP8266_Get_LinkSta())
    {
			//OLED_DispStr(0,  45, "                  ", &tFont12);//��ղ�����ʾ
			
      case 1 :	
				printf("�ӻ�����\r\n");
				OLED_DispStr(0,  30, "Connected", &tFont12);
			break;
	    case 2 :  
				printf("�ӻ�δ����\r\n");
				OLED_DispStr(0,  30, "Disconnected", &tFont12);
			
			break;
				
    }
    if(ESP8266_Get_Data(Data_buff,Data_len,Data_id))
    {
      printf("����id:%s��%s�ֽ����ݵ���\r\n",Data_id,Data_len);
      printf("����:%s\r\n",Data_buff);
			
			if(Data_buff[0]==0X5A && Data_buff[1]==0X01)//������׵�ָ��
			{
				Return_Data[0]=0xA5;//�������ݵ�֡ͷ
				if(Queue_Head_Inquire()==Queue_Tail_Inquire())//������׺Ͷ�β��ȣ�˵��û�пͻ�
					Return_Data[1]=0;//����0���ӻ�
				else
				{
					Queue_Head_Depart();
					Return_Data[1]=Queue_Head_Inquire();
				}
				ESP8266_SendData(Char_to_Hex(Data_id,strlen((const char *)Data_id)),Return_Data,2,50);//5s�ķ��ͳ�ʱʱ��		����1 ���� ����2 ���� ��ʱʱ��
			}
			
    }
  }		
}
