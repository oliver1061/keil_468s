#include "time.h"
#include "gpioconfig.h"
#include "delay.h"

extern volatile uint32_t arv[ARG];        //采样缓冲区
extern volatile uint16_t plase;           //缓冲区指针
extern volatile uint16_t updata_val;      //更新中断次数
extern volatile uint8_t flag;             //采样完成标志

extern volatile uint8_t voltage_low;

//PB1 TIME3_CH4
void Time3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	
	//打开定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//引脚配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);     //引脚复用配置
	
	//定时器配置
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;         //定时器分频
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	

	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;                  //通道
	TIM_ICInitStruct.TIM_ICFilter = 0;                             //滤波
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;       //极性 上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV8;             //分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;   //捕获选择
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);      //允许捕获中断
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);   //允许更新中断
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_Cmd(TIM3, DISABLE);
}

void TIM14_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//打开定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	
	
	//定时器配置   //25ms中断一次
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 2499;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 479;         //定时器分频
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
//	TIM_ITConfig(TIM14, TIM_IT_Update, DISABLE);   
//	TIM_Cmd(TIM14, DISABLE);
	
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);   //允许更新中断
	TIM_Cmd(TIM14, ENABLE);                       //使能定时器
}

//定时器14中断服务程序
void TIM14_IRQHandler(void)   //TIM14中断
{
	static uint16_t irq_mun = 0;
	
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		if(voltage_low)
		{
			irq_mun++;
			if(irq_mun>4)
			{
				irq_mun = 0;
				LED_POWER_TOGLE;
			}
		}
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	
	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)  //检查TIM3通道4捕获中断发生与否
	{
		arv[plase] = (updata_val * 0xffff) + (TIM3->CCR4);
		TIM_SetCounter(TIM3, 0);     //定时器清零
		
		updata_val = 0;              //溢出次数清零
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);  //清除TIM3通道4捕获中断标志 

		plase++;
		if(plase >= ARG)    //完成一轮采样
		{
			plase = 0;
			
			TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);      //允许捕获中断
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);   //允许更新中断
			TIM_Cmd(TIM3, DISABLE);                       //使能定时器
			flag = 1;                                     //标志位置1
		}
	}
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		updata_val++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
	
}


