#include "select.h"

void Channel_Select_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruction = {0};
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; //GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12 
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruction.GPIO_OType = GPIO_OType_PP;    //必须选推挽输出，否则烧录不了，且不能仿真
	GPIO_Init(GPIOA,&GPIO_InitStruction);
	
}

void Channel_Select(uint8_t channel)
{
	if(channel & 0x01)
	{
		GPIOA->ODR |= GPIO_Pin_12;
	}
	else
	{
		GPIOA->ODR &= ~GPIO_Pin_12;
	}
	
	if(channel & 0x02)
	{
		GPIOA->ODR |= GPIO_Pin_11;
	}
	else
	{
		GPIOA->ODR &= ~GPIO_Pin_11;
	}
	
	if(channel & 0x04)
	{
		GPIOA->ODR |= GPIO_Pin_10;
	}
	else
	{
		GPIOA->ODR &= ~GPIO_Pin_10;
	}
}



