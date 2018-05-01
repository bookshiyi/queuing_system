/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��led.c
 * ����    ��LED����������     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: D1->PC13;D2->PB0;D3->PB1
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "buzz.h"
#include "delay.h"
 u8 Beep_flag=0;
 /**
  * @file   LED_GPIO_Config
  * @brief  LED����������
  * @param  ��
  * @retval ��
  */
void BUZZ_GPIO_Config(void)
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;	
    RCC_APB2PeriphClockCmd(BUZZ_GPIO_RCC,ENABLE);//ʹ��GPIO������ʱ��
    /*BUZZ*/
    GPIO_InitStructure.GPIO_Pin =BUZZ_GPIO_PIN;//ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
    GPIO_Init(BUZZ_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO

}

 /**
  * @file   LEDXToggle
  * @brief  LED����ת
  * @param  ledx:1--PC13;2--PB0;3--PB1
  * @retval ��
  */
void Beep(uint16_t time)
{
	uint16_t i;
 for(i=0;i<time;i++)
	{
		BUZZ_ONOFF(1);
		delay_us(500);
		BUZZ_ONOFF(0);
		delay_us(500);
	}

}

