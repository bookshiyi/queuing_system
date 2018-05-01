#ifndef __EXTI_H
#define __EXTI_H


#include "stm32f10x.h"

#define KEY1_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY1_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource5
#define KEY1_BUTTON_EXTI_LINE             EXTI_Line5
#define KEY1_BUTTON_EXTI_IRQn             EXTI9_5_IRQn


#define KEY2_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY2_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource8
#define KEY2_BUTTON_EXTI_LINE             EXTI_Line8
#define KEY2_BUTTON_EXTI_IRQn             EXTI9_5_IRQn

#define KEY3_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY3_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource9
#define KEY3_BUTTON_EXTI_LINE             EXTI_Line9
#define KEY3_BUTTON_EXTI_IRQn             EXTI9_5_IRQn


void Exti_Config(u8 status);
void Nvic_Config(void);
void EXTI9_5_IRQHandler(void);
//void EXTI15_10_IRQHandler(void);

#endif
