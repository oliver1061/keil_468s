#include "time.h"
#include "gpioconfig.h"
#include "delay.h"

extern volatile uint32_t arv[ARG];        //����������
extern volatile uint16_t plase;           //������ָ��
extern volatile uint16_t updata_val;      //�����жϴ���
extern volatile uint8_t flag;             //������ɱ�־

extern volatile uint8_t voltage_low;

//PB1 TIME3_CH4
void Time3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	
	//�򿪶�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);     //���Ÿ�������
	
	//��ʱ������
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;         //��ʱ����Ƶ
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	

	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;                  //ͨ��
	TIM_ICInitStruct.TIM_ICFilter = 0;                             //�˲�
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;       //���� �����ش���
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV8;             //��Ƶ
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;   //����ѡ��
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);      //�������ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);   //��������ж�
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_Cmd(TIM3, DISABLE);
}

void TIM14_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�򿪶�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	
	
	//��ʱ������   //25ms�ж�һ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = 2499;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 479;         //��ʱ����Ƶ
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
//	TIM_ITConfig(TIM14, TIM_IT_Update, DISABLE);   
//	TIM_Cmd(TIM14, DISABLE);
	
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);   //��������ж�
	TIM_Cmd(TIM14, ENABLE);                       //ʹ�ܶ�ʱ��
}

//��ʱ��14�жϷ������
void TIM14_IRQHandler(void)   //TIM14�ж�
{
	static uint16_t irq_mun = 0;
	
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
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
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	
	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)  //���TIM3ͨ��4�����жϷ������
	{
		arv[plase] = (updata_val * 0xffff) + (TIM3->CCR4);
		TIM_SetCounter(TIM3, 0);     //��ʱ������
		
		updata_val = 0;              //�����������
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);  //���TIM3ͨ��4�����жϱ�־ 

		plase++;
		if(plase >= ARG)    //���һ�ֲ���
		{
			plase = 0;
			
			TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);      //�������ж�
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);   //��������ж�
			TIM_Cmd(TIM3, DISABLE);                       //ʹ�ܶ�ʱ��
			flag = 1;                                     //��־λ��1
		}
	}
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		updata_val++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
	
}


