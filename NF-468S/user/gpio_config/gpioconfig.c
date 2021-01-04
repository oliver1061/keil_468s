#include "gpioconfig.h"

void GPIO_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruction = {0};
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF, ENABLE);
		
/*******************************压线LED灯初始化*********************************/
	LED_Ctl(0x00); //关闭压线灯
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruction.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruction.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruction);	
	
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruction);	
		
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStruction);	
/**************************************************************************/

/*******************************蜂鸣器初始化*********************************/
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_Init(GPIOA,&GPIO_InitStruction);	
/**************************************************************************/
	
/*******************************电源灯初始化*********************************/
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStruction);	
	
/****************************************************************************/


/*******************************压线/远端选择控制引脚*********************************/
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_15;
	GPIO_Init(GPIOA,&GPIO_InitStruction);	
	
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruction);	
	
	
/****************************************************************************/


/*******************************压线/远端选择检测引脚*********************************/
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_IN;                  //模式切换输入引脚
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_1;
	GPIO_InitStruction.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruction.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruction);	
/****************************************************************************/


/*******************************压线校准引脚*********************************/
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_IN;                  //压线校准引脚
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_Level_1;
	GPIO_InitStruction.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruction.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF,&GPIO_InitStruction);	
/****************************************************************************/
	
	
}	


void LED_Ctl(uint8_t data)
{
	if(data & (1<<0))
		LED3_ON;
	else
		LED3_OFF;
	
	if(data & (1<<1))
		LED4_ON;
	else
		LED4_OFF;
	
	if(data & (1<<2))
		LED6_ON;
	else
		LED6_OFF;
	
	if(data & (1<<3))
		LED7_ON;
	else
		LED7_OFF;
	
	if(data & (1<<4))
		LED8_ON;
	else
		LED8_OFF;
	
	if(data & (1<<5))
		LED9_ON;
	else
		LED9_OFF;
	
	if(data & (1<<6))
		LED10_ON;
	else
		LED10_OFF;
	
	if(data & (1<<7))
		LED11_ON;
	else
		LED11_OFF;
	
}



