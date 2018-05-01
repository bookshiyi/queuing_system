/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart.c
 * ����    ������1�����úʹ���1�жϴ���     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: TX->PA9;RX->PA10
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

#include "usart.h"	  

//��������
void NVIC_Config1(void);


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];  //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15,������ɱ�־
//bit14,���յ�0x0d
//bit13~0,���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;//����״̬���	  

 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO ����,����ģʽ����
  * @param  bound��������
  * @retval ��
  */
void USART1_Config(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  USART_DeInit(USART1);  //��λ����1
  /*USART1_TX ->PA9*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
  /*USART1_RX ->PA10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
  //USART ��ʼ������
  USART_InitStructure.USART_BaudRate = bound;//������;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ��8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ��1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ����
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܽ��պͷ�������
   /*�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��*/
  USART_Init(USART1, &USART_InitStructure); //��ʼ������
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

  NVIC_Config1();// Usart1 NVIC �ж���������
}

 /**
  * @file   NVIC_Config1
  * @brief  Usart1 NVIC �ж���������
  * @param  ��
  * @retval ��
  */
void NVIC_Config1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


 /**
  * @file   USART1_IRQHandler
  * @brief  ����1�жϷ������
  * @param  ��
  * @retval ��
  */
void USART1_IRQHandler(void)                
{
  u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
    Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
    if((USART_RX_STA&0x8000)==0)//����δ���
    {
      if(USART_RX_STA&0x4000)//���յ���0x0d
      {
        if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
        else USART_RX_STA|=0x8000;	//��������� 
      }
      else //��û�յ�0X0D
      {	
        if(Res==0x0d)USART_RX_STA|=0x4000;
        else
        {
          USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
          USART_RX_STA++;
          if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
        }		 
      }
    }   		 
  } 
} 
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
  return ch;
}


