#ifndef _GPIOCONFIG_H_
#define _GPIOCONFIG_H_

#include "stm32f0xx.h"

#define      LED_POWER_ON        GPIOA->ODR &= ~GPIO_Pin_8
#define      LED_POWER_OFF       GPIOA->ODR |=  GPIO_Pin_8
#define      LED_POWER_TOGLE     GPIOA->ODR ^=  GPIO_Pin_8
//Ñ¹Ïß
#define      SW_STATE_1 \
							GPIOA->ODR |=  GPIO_Pin_15;\
							GPIOB->ODR &= ~GPIO_Pin_6;\
							GPIOB->ODR &= ~GPIO_Pin_7
//Ô¶¶Ë
#define      SW_STATE_2 \
							GPIOA->ODR &= ~GPIO_Pin_15;\
							GPIOB->ODR |=  GPIO_Pin_6;\
							GPIOB->ODR |=  GPIO_Pin_7	
							
#define      LED3_ON      GPIOF->ODR &= ~GPIO_Pin_1
#define      LED4_ON      GPIOA->ODR &= ~GPIO_Pin_2
#define      LED6_ON      GPIOA->ODR &= ~GPIO_Pin_3
#define      LED7_ON      GPIOA->ODR &= ~GPIO_Pin_4
#define      LED8_ON      GPIOA->ODR &= ~GPIO_Pin_5
#define      LED9_ON      GPIOA->ODR &= ~GPIO_Pin_6
#define      LED10_ON     GPIOA->ODR &= ~GPIO_Pin_7
#define      LED11_ON     GPIOB->ODR &= ~GPIO_Pin_0

#define      LED3_OFF      GPIOF->ODR |= GPIO_Pin_1
#define      LED4_OFF      GPIOA->ODR |= GPIO_Pin_2
#define      LED6_OFF      GPIOA->ODR |= GPIO_Pin_3
#define      LED7_OFF      GPIOA->ODR |= GPIO_Pin_4
#define      LED8_OFF      GPIOA->ODR |= GPIO_Pin_5
#define      LED9_OFF      GPIOA->ODR |= GPIO_Pin_6
#define      LED10_OFF     GPIOA->ODR |= GPIO_Pin_7
#define      LED11_OFF     GPIOB->ODR |= GPIO_Pin_0

#define      BUZZER_ON     GPIOA->ODR ^=  GPIO_Pin_9

void GPIO_InitConfig(void);
void LED_Ctl(uint8_t data);
void PB1_Init(void);

#endif //_GPIOCONFIG_H_



