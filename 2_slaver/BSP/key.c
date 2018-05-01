/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��key.c
 * ����    ��������������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������  :S1->PB5;S2->PB8;S3->PB9
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

// ͷ�ļ�
#include "key.h"

 /**
  * @file   KEY_GPIO_Config
  * @brief  ������������
  * @param  ��
  * @retval ��
  */
void KEY_GPIO_Config(void)
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(KEY1_BUTTON_GPIO_RCC|KEY2_BUTTON_GPIO_RCC|
                           KEY3_BUTTON_GPIO_RCC,ENABLE);//����GPIOC������ʱ��
    
    GPIO_InitStructure.GPIO_Pin = KEY1_BUTTON_PIN;//ѡ��Ҫ�õ�GPIO����	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������ģʽΪ��������ģʽ				 
    GPIO_Init(KEY1_BUTTON_GPIO_PORT, &GPIO_InitStructure); //���ÿ⺯������ʼ��GPIO   

    GPIO_InitStructure.GPIO_Pin = KEY2_BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 
    GPIO_Init(KEY2_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = KEY3_BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 
    GPIO_Init(KEY3_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
	
}

