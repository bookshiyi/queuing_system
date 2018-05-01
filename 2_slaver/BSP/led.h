#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1                     1
#define LED2                     2
#define LED3                     3

#define LED1_GPIO_RCC           RCC_APB2Periph_GPIOC
#define LED1_GPIO_PORT          GPIOC
#define LED1_GPIO_PIN      			GPIO_Pin_13
#define LED1_ONOFF(x)     			GPIO_WriteBit(GPIOC,GPIO_Pin_13,x);

#define LED2_GPIO_RCC           RCC_APB2Periph_GPIOB
#define LED2_GPIO_PORT          GPIOB
#define LED2_GPIO_PIN      			GPIO_Pin_0
#define LED2_ONOFF(x)     			GPIO_WriteBit(GPIOB,GPIO_Pin_0,x);

#define LED3_GPIO_RCC           RCC_APB2Periph_GPIOB
#define LED3_GPIO_PORT          GPIOB
#define LED3_GPIO_PIN      			GPIO_Pin_1
#define LED3_ONOFF(x)      			GPIO_WriteBit(GPIOB,GPIO_Pin_1,x);


void LED_GPIO_Config(void);	
void LEDXToggle(uint8_t ledx);
#endif
