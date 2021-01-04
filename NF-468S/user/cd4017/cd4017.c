#include "cd4017.h"

void CD4017_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruction = {0};
	
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_Init(GPIOB,&GPIO_InitStruction);	
	
//	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_6;
//	GPIO_Init(GPIOB,&GPIO_InitStruction);	
}




