/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：led.c
 * 描述    ：LED灯引脚配置     
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: D1->PC13;D2->PB0;D3->PB1
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
#include "buzz.h"
#include "delay.h"
 u8 Beep_flag=0;
 /**
  * @file   LED_GPIO_Config
  * @brief  LED灯引脚配置
  * @param  无
  * @retval 无
  */
void BUZZ_GPIO_Config(void)
{	
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;	
    RCC_APB2PeriphClockCmd(BUZZ_GPIO_RCC,ENABLE);//使能GPIO的外设时钟
    /*BUZZ*/
    GPIO_InitStructure.GPIO_Pin =BUZZ_GPIO_PIN;//选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
    GPIO_Init(BUZZ_GPIO_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO

}

 /**
  * @file   LEDXToggle
  * @brief  LED亮灭翻转
  * @param  ledx:1--PC13;2--PB0;3--PB1
  * @retval 无
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

