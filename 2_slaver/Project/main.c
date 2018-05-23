/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    :�������У�ESP8266ģ�鹤����STA���ͻ��ˣ�ģʽ�����ն�ģʽ����Ҫ��������·��������������APģʽ  
 * ��汾  ��V3.50
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * ���Է�ʽ��J-Link-OB
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

u8  Data_buff[64];     //���ݻ�����
u8  Data_len[5];         //���ݳ��Ȼ�����
u8  Data_id[5];          //���ݷ����ߵ�id������


u8 Send_Data[64];		//�������������ݵĻ���

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

  SYSTICK_Init();	    //��ʱ������ʼ��
  LED_GPIO_Config();
	BUZZ_GPIO_Config();
	KEY_GPIO_Config();////////////////////////////////////
	//Exti_Config();//�����ж�����
  //Nvic_Config();//???????�����ж���������
	
  delay_ms(1500);     //�ȴ�оƬ�ϵ縴λ�ɹ�
	OLED_Init(); 
  USART1_Config(9600);	//���ڳ�ʼ��
  uart2_init(115200);	//���ڳ�ʼ��
	
	delay_ms(1000);
	printf("<<Z>0"); 
	delay_ms(3000);
	printf("<V>4"); 	//��������/////////////////////////////
	delay_ms(2000);

	

	OLED_DispStr(0, 15, "  Slaver OK  ", &tFont12);
	OLED_DispStr(0, 30, "------------------", &tFont12);
	OLED_DispStr(15, 45, "Please Wait...", &tFont12);     
	

  ESP8266_Exit_Transfer(10);//����Ƿ���͸��״̬����������˳�
  printf("\r\n��������\r\n");

  res1=ESP8266_STA_TCPCLient();
  switch(res1)
  {
    case 0  :  printf("����͸��\r\n");
								OLED_DispStr(0, 45, "Connected Success", &tFont12);
								break;
    case 1  :  printf("�����λʧ��! ׼������\r\n");
								OLED_DispStr(0, 45, "S-reset failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 2  :  printf("����STAģʽʧ��! ׼������\r\n");
								OLED_DispStr(0, 45, "STA failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 3  :  printf("����APʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "C-AP failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 4  :  printf("��IP��ȡʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "IP failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 5  :  printf("����͸��ʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "transmission failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 6  :  printf("�رն�·����ʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "Multiple failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 7  :  printf("��������ʧ�ܣ�����δ������׼������\r\n");
								OLED_DispStr(0, 45, "Master Closed", &tFont12);
								;break;
    case 8  :  printf("����������\r\n");
								OLED_DispStr(0, 45, "Connected", &tFont12);
								break;
    case 9  :  printf("��������ʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "AP-failed failed", &tFont12);
								NVIC_SystemReset();
								break;
    case 10 :  printf("����͸��ʧ��!,׼������\r\n");
								OLED_DispStr(0, 45, "transmission failed", &tFont12);
								NVIC_SystemReset();
								break;
  }
  if(res1==7)
  {
      res2=1;
      
      while(res2)
      {
        res2=ESP8266_Connect_Server(SERVER_IP,SERVER_PORT,50);     //����TCP������,������͸������ʱʱ��5s��ʧ�ܷ���7
        switch(res2)
        {
          case 0 :  printf("����͸��\r\n");break;//���������ɹ�
          case 1 :  printf("��������ʧ�ܣ�����δ������׼������\r\n");break;
          case 2 :  printf("����������\r\n");break;
          case 3 :  printf("��������ʧ��!,׼������\r\n");NVIC_SystemReset();break;
          case 4 :  printf("����͸��ʧ��!,׼������\r\n");NVIC_SystemReset();break;
        }
        for(i=0;i<20;i++)
        delay_ms(1000);    //��ʱ20s
      }		
  }
  USART2_DATA_FLAG=1;  //��ǽ�������͸����
  USART2_RX_STA=0;  
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //�������2����
  while(1)
  {
		
		if((KEY1_Status_Read() == 0) || (KEY2_Status_Read() == 0) || (KEY3_Status_Read() == 0))
		{			
			delay_ms(10);
			if((KEY1_Status_Read() == 0) || (KEY2_Status_Read() == 0) || (KEY3_Status_Read() == 0))
			{
				Send_Data[0]='Q';
				Send_Data[1]='?';//������׵�����
				Send_Data[2]='\0';
				
				u2_printf("%s",Send_Data);//���ݷ��͸�����

				Beep(200);
				LEDXToggle(LED1);LEDXToggle(LED2);LEDXToggle(LED3);
			}
		}
		
		
		
    if(USART2_RX_STA&0x8000)//����յ���������������
    {
      len=USART2_RX_STA&0x7FFF;
      memcpy(Data_buff,USART2_RX_BUF,len);
			memset(USART2_RX_BUF,0,USART2_REC_LEN); //�������2����
      USART2_RX_STA=0;		
      Data_buff[len]='\0';
			
			//printf("�յ�����%s\r\n",Data_buff);//����ʹ��
			
			if('Q'==Data_buff[0] )
			{
				if(('0'==Data_buff[1])&&('0'==Data_buff[2])&&('0'==Data_buff[3]))//��ʱ�����Ŷ�
				{
					printf("��ʱû���Ŷӿͻ�\r\n");
					OLED_DispStr(0, 45, "��ʱû���Ŷӿͻ�  ", &tFont12);//�����Ŷ�
				}
				else//�пͻ��Ŷ�
				{
					num[0]=Data_buff[1];//�ַ�תascii
					num[1]=Data_buff[2];
					num[2]=Data_buff[3];
					num[3]='\0';
					printf("<Z>1");
					delay_ms(1500);
					printf("<Z>4");
					delay_ms(1500);
					OLED_DispStr(0, 45, "���Ӵ�    �ſͻ�   ", &tFont12);//�����Ŷ�
					OLED_DispStr(42, 45,num, &tFont12);//oled��ʾ
					printf("��%s�ſͻ�����ҵ��\r\n",num);
				}
			}
			else
			{
					OLED_DispStr(0, 45, "ͨѶ����", &tFont12);//ͨ�ų�������
					OLED_DisDigital(60, 45,*num, &tFont12);//oled��ʾ
					printf("ͨѶ����\r\n");
			}
    }


  }
}

