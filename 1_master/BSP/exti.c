/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��exti.c
 * ����    ��������������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/
//ͷ�ļ�  
#include "exti.h"
#include "led.h"
#include "buzz.h"
#include "delay.h"

//��������
void Delay(u32 n);

 /**
  * @file   Exti_Config
  * @brief  ���ú����⣬��ʼ���ж�����
  * @param  ��
  * @retval ��
  */
void Exti_Config(u8 status)
{	
	/*�鿴�ⲿ�ж�/�¼���·ӳ��
  PA0��PB0��PC0��PD0��PE0��PF0��PG0 ��������>ӳ���� EXTI0
  PA1��PB1��PC1��PD1��PE1��PF1��PG1 ��������>ӳ���� EXTI1
  PA2��PB2��PC2��PD2��PE2��PF2��PG2 ��������>ӳ���� EXTI2
  PA3��PB3��PC3��PD3��PE3��PF3��PG3 ��������>ӳ���� EXTI3
  .......�������ƣ�ÿ���ⲿ�ж϶���һ���¼���·ӳ��*/
  //����һ��EXTI_InitTypeDef ���͵Ľṹ��
  EXTI_InitTypeDef EXTI_InitStructure;
  //ʹ��AFIO���ܵ�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	
  //��֪�ж���·
  GPIO_EXTILineConfig(KEY1_BUTTON_EXTI_PORT_SOURCE, KEY1_BUTTON_EXTI_PIN_SOURCE);
  GPIO_EXTILineConfig(KEY2_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE);
  GPIO_EXTILineConfig(KEY3_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE);
  //ѡ���ж���·
  EXTI_InitStructure.EXTI_Line = KEY1_BUTTON_EXTI_LINE | KEY2_BUTTON_EXTI_LINE | KEY3_BUTTON_EXTI_LINE; 
  //����Ϊ�ж����󴥷�ģʽ
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //����Ϊ�½��ش���
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  //����Ϊʹ���ж���
	if(status == 1)
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  //���ÿ⺯������ʼ���ж�
  EXTI_Init(&EXTI_InitStructure);
}
/**
  * @file   nvic_config
  * @brief  ���ÿ⺯������ʼ���ж�����
  * @param  ��
  * @retval ��
  */
void Nvic_Config()
{
 
  NVIC_InitTypeDef NVIC_InitStructure; //����һ��NVIC_InitTypeDef ���͵Ľṹ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //ѡ���жϷ���0	
	
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_BUTTON_EXTI_IRQn|KEY2_BUTTON_EXTI_IRQn|KEY3_BUTTON_EXTI_IRQn;//����ѡ�е��ж�����,ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //������ռ���ȼ�:��ռʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //������Ӧ���ȼ�:��Ӧʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ж�����
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC
}

/**
  * @file   EXTI9_5_IRQHandler
  * @brief  �жϴ�����
  * @param  ��
  * @retval ��
  */
void EXTI9_5_IRQHandler(void)
{  

/*���ָ����EXTI��·�������������*/
  if (EXTI_GetITStatus(KEY1_BUTTON_EXTI_LINE) != RESET)
  { 
			EXTI->IMR &= ~(KEY1_BUTTON_EXTI_LINE); 
    /*���EXTI��·����λ*/
    EXTI_ClearITPendingBit(KEY1_BUTTON_EXTI_LINE); 
   /*D1״̬��ת*/
   LEDXToggle(LED1);
		Beep_flag = 1;
  }
  /*���ָ����EXTI��·�������������*/
  if (EXTI_GetITStatus(KEY2_BUTTON_EXTI_LINE) != RESET)
  {
			EXTI->IMR &= ~(KEY2_BUTTON_EXTI_LINE); 
    /*���EXTI��·����λ*/
    EXTI_ClearITPendingBit(KEY2_BUTTON_EXTI_LINE);
   /*D2״̬��ת*/
   LEDXToggle(LED2);
		Beep_flag = 1;
  }	
  /*���ָ����EXTI��·�������������*/
  if (EXTI_GetITStatus(KEY3_BUTTON_EXTI_LINE) != RESET)
  { 
    /*���EXTI��·����λ*/
		EXTI->IMR &= ~(KEY3_BUTTON_EXTI_LINE); 
    EXTI_ClearITPendingBit(KEY3_BUTTON_EXTI_LINE); 
  /*D4״̬��ת*/
   LEDXToggle(LED3);
		Beep_flag = 1;
  }
	EXTI->IMR |= EXTI_Line5;	
	EXTI->IMR |= EXTI_Line8;
	EXTI->IMR |= EXTI_Line9;
}

