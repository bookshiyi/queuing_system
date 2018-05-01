/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：key.c
 * 描述    ：按键引脚配置
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接  :S1->PB5;S2->PB8;S3->PB9
 * 调试方式：J-Link-OB
**********************************************************************************/

// 头文件
#include "key.h"

 /**
  * @file   KEY_GPIO_Config
  * @brief  按键引脚配置
  * @param  无
  * @retval 无
  */
void KEY_GPIO_Config(void)
{	
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(KEY1_BUTTON_GPIO_RCC|KEY2_BUTTON_GPIO_RCC|
                           KEY3_BUTTON_GPIO_RCC,ENABLE);//开启GPIOC的外设时钟
    
    GPIO_InitStructure.GPIO_Pin = KEY1_BUTTON_PIN;//选择要用的GPIO引脚	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置引脚模式为上拉输入模式				 
    GPIO_Init(KEY1_BUTTON_GPIO_PORT, &GPIO_InitStructure); //调用库函数，初始化GPIO   

    GPIO_InitStructure.GPIO_Pin = KEY2_BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 
    GPIO_Init(KEY2_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = KEY3_BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 
    GPIO_Init(KEY3_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
	
}

