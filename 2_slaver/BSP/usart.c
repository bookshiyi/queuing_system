/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：usart.c
 * 描述    ：串口1的配置和串口1中断处理     
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: TX->PA9;RX->PA10
 * 调试方式：J-Link-OB
**********************************************************************************/	

#include "usart.h"	  

//函数声明
void NVIC_Config1(void);


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];  //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15,接收完成标志
//bit14,接收到0x0d
//bit13~0,接收到的有效字节数目
u16 USART_RX_STA=0;//接收状态标记	  

 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO 配置,工作模式配置
  * @param  bound：波特率
  * @retval 无
  */
void USART1_Config(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  USART_DeInit(USART1);  //复位串口1
  /*USART1_TX ->PA9*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
  /*USART1_RX ->PA10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
  //USART 初始化设置
  USART_InitStructure.USART_BaudRate = bound;//波特率;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位：8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位：1位
  USART_InitStructure.USART_Parity = USART_Parity_No;//校验位：无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能接收和发送引脚
   /*将以上赋完值的结构体带入库函数USART_Init进行初始化*/
  USART_Init(USART1, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口中断接收
  USART_Cmd(USART1, ENABLE);                    //使能串口 

  NVIC_Config1();// Usart1 NVIC 中断向量配置
}

 /**
  * @file   NVIC_Config1
  * @brief  Usart1 NVIC 中断向量配置
  * @param  无
  * @retval 无
  */
void NVIC_Config1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


 /**
  * @file   USART1_IRQHandler
  * @brief  串口1中断服务程序
  * @param  无
  * @retval 无
  */
void USART1_IRQHandler(void)                
{
  u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
  {
    Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
    if((USART_RX_STA&0x8000)==0)//接收未完成
    {
      if(USART_RX_STA&0x4000)//接收到了0x0d
      {
        if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
        else USART_RX_STA|=0x8000;	//接收完成了 
      }
      else //还没收到0X0D
      {	
        if(Res==0x0d)USART_RX_STA|=0x4000;
        else
        {
          USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
          USART_RX_STA++;
          if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
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


