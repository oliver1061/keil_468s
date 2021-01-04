#ifndef _CD4017_H_
#define _CD4017_H_

#include "stm32f0xx.h"

#define          CLK_H                GPIOB->ODR |= GPIO_Pin_4
#define          CLK_L                GPIOB->ODR &= ~GPIO_Pin_4

#define          RSE_H                GPIOB->ODR |= GPIO_Pin_3
#define          RSE_L                GPIOB->ODR &= ~GPIO_Pin_3

void CD4017_Init(void);


#endif //_CD4017_H_



