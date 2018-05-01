#ifndef __USART_H
#define __USART_H


#include "stm32f10x.h"
#include "stdio.h"	

#define USART_REC_LEN  			256  	//�����������ֽ��� 256
#define EN_USART1_RX 			  1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void USART1_Config(u32 bound);
#endif


