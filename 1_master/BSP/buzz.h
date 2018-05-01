#ifndef __BUZZ_H
#define __BUZZ_H

#include "stm32f10x.h"

extern u8 Beep_flag;

#define BUZZ_GPIO_RCC           RCC_APB2Periph_GPIOA
#define BUZZ_GPIO_PORT          GPIOA
#define BUZZ_GPIO_PIN      			GPIO_Pin_1
#define BUZZ_ONOFF(x)     			GPIO_WriteBit(GPIOA,GPIO_Pin_1,x);



void BUZZ_GPIO_Config(void);	
void Beep(uint16_t time);
#endif
