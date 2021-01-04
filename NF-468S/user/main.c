#include "gpioconfig.h"
#include "time.h"
#include "select.h"
#include "delay.h"
#include "cd4017.h"
#include "adc.h"

//V1.1  改校准MIN阀值  ADJ_DATA_MIN 由 18900 改为 16500
//V1.2  试一下能不能正常提交修改后的内容


//Schmitt
#define    THRESHOLD      50     //阀值
#define    QUAN           3      //数量

#define ADJ_DATA_MAX        22100
#define ADJ_DATA_MIN        16500


typedef struct Schmitt
{
	uint16_t difference[QUAN];      //差值
	uint8_t thre_qua[QUAN];         //达到阀值的数量
	uint8_t not_thre_qua[QUAN];     //达不到阀值的数量
	uint8_t count_p;                //计数指针
}schmitt;

schmitt buzzer = {0};

volatile uint32_t arv[ARG];        //采样缓冲区
volatile uint16_t plase;           //缓冲区指针
volatile uint16_t updata_val;      //更新中断次数
volatile uint8_t flag = 0;         //采样完成标志
volatile uint16_t ADC1ConvertedValue[2];   //ADC数值
volatile uint8_t voltage_low = 0;          //低电压标志
uint8_t test_num = 9;                      //远端计数

	
uint32_t total;
uint32_t thresholds[8];    //阀值

uint16_t set=0;

	//阀值校准函数
uint32_t data[8];

/**************************函数声明*********************************/
static void Write_Flash(uint32_t data[]);
static void adjusting(uint32_t data[]);
static void Read_Flash(uint32_t thresholds[]);
static void ADC_test(void);

/********************************************************************/



int main()
{
	int i, j;
	
	//初始化程序
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	
	Channel_Select_Init();
	GPIO_InitConfig();
	CD4017_Init();
	Time3();
	TIM14_Init();
	ADC_InitConfig();
	
	//adjusting(data);
	//Write_Flash(data);        //写flash
//	LED_Ctl(0x00);
//	delay_ms(200);
//	LED_Ctl(0xff);
//	delay_ms(200);
//	LED_Ctl(0x00);
	
						
	//读阀值
	Read_Flash(thresholds);
	
#if 0
	GPIOA->ODR |= GPIO_Pin_15;    //切换压线测试
	GPIOB->ODR &= ~GPIO_Pin_6;
	GPIOB->ODR &= ~GPIO_Pin_7;
	LED_POWER_ON;    //开灯
	
	Channel_Select(3);
	delay_ms(30);
	
	
	while(1)
	{
		plase = 0;
		flag = 0;
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //允许捕获中断
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //允许更新中断
		TIM_Cmd(TIM3, ENABLE); //开定时器
		while(!flag);
		
		delay_ms(200);
	}
#endif
	
	while(1)
	{
		ADC_test();
	
		if(voltage_low)    //如果电压低
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)    //压线
			{
				GPIOA->ODR |= GPIO_Pin_15;    //切换压线测试
				GPIOB->ODR &= ~GPIO_Pin_6;
				GPIOB->ODR &= ~GPIO_Pin_7;
				
				if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)       //压线校准
				{
					delay_ms(500);
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)
					{
						adjusting(data);
						
						Write_Flash(data);        //写flash
						LED_Ctl(0x00);
						delay_ms(200);
						LED_Ctl(0xff);
						delay_ms(200);
						LED_Ctl(0x00);
						
						Read_Flash(thresholds);    //读阀值
						
						while(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0);    //等待松开
					}
				}
				
				
				set = 0;
				
				for(i=0; i<8; i++)
				{
					Channel_Select(i);
					delay_ms(30);
					
					plase = 0;
					flag = 0;
					
					TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //允许捕获中断
					TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //允许更新中断
					TIM_Cmd(TIM3, ENABLE); //开定时器
					
					while(!flag);
					
					
					for(total=0, j=(ARG*0.25); j<(ARG*0.75); j++)
					{
						total += arv[j];
					}
					
					if(total > thresholds[i])
					{
						set |= 1<<i;
					}
				}
				LED_Ctl(set);
				delay_ms(1);
			}
			
			
			else        //远端
			{
				SW_STATE_2;
				
				if(test_num >= 10)
				{
					test_num = 0;
					RSE_L;      
					delay_ms(1);
					RSE_H;
					delay_ms(1);
				}
				test_num++;
				delay_ms(250);
				CLK_L;
				delay_ms(1);
				CLK_H;
				delay_ms(1);
				delay_ms(250);
			}
		}
		
		else    //电压充足
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)    //压线
			{
				GPIOA->ODR |= GPIO_Pin_15;    //切换压线测试
				GPIOB->ODR &= ~GPIO_Pin_6;
				GPIOB->ODR &= ~GPIO_Pin_7;
				LED_POWER_ON;    //开灯
				
				if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)       //压线校准
				{
					delay_ms(500);
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)
					{
						adjusting(data);
						
						while(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0);    //等待松开
					}
				}
				
				
				set = 0;
				
				for(i=0; i<8; i++)
				{
					Channel_Select(i);
					delay_ms(30);
					
					plase = 0;
					flag = 0;
					
					TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //允许捕获中断
					TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //允许更新中断
					TIM_Cmd(TIM3, ENABLE); //开定时器
					
					while(!flag);
					
					
					for(total=0, j=(ARG*0.25); j<(ARG*0.75); j++)
					{
						total += arv[j];
					}
					
					if(total > thresholds[i])
					{
						set |= 1<<i;
					}
				}
				LED_Ctl(set);
				delay_ms(1);
			}
			
			
			else        //远端
			{
				GPIOA->ODR &= ~GPIO_Pin_15;  //切换远端模式
				GPIOB->ODR |= GPIO_Pin_6;   //PB6高
				GPIOB->ODR |= GPIO_Pin_7;
				
				
				if(test_num >= 10)
				{
					test_num = 0;
					RSE_L;      
					delay_ms(1);
					RSE_H;
					delay_ms(1);
				}
				test_num++;
				LED_POWER_OFF;     //关灯
				delay_ms(250);
				CLK_L;
				delay_ms(1);
				CLK_H;
				delay_ms(1);
				LED_POWER_ON;    //开灯
				delay_ms(250);
				
			}
		}
	

	}
}

static void ADC_test(void)
{
	
	
	
		while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
		DMA_ClearFlag(DMA1_FLAG_TC1);
	
	
	
	if(ADC1ConvertedValue[0] < 1860)    //电压低于6.5 2135  1970
	{
		voltage_low = 1;
	}
		
	else    //电压高于6.8  2335  2076
	{
		voltage_low = 0;
	}
		
	
}

	//写flash函数
static void Write_Flash(uint32_t data[])
{
	int i;
	
			//写flash
		//1.解锁
		FLASH_Unlock();
		//2.擦除
		FLASH_ErasePage(0x08000000 | (0x400*15));
		//3.写数据
		for(i=0; i<8; i++)
		{
			FLASH_ProgramWord(0x08000000 | (0x400*15) + 4*i, data[i]);
		}
		
		//4.写完上锁
		FLASH_Lock();
}

static void adjusting(uint32_t data[])
{
	int i,j;
	uint8_t led_set = 0;
	
	delay_ms(500);
	
	//校准提示
	LED_Ctl(0x00);
	for(i=0; i<8; i++)
	{
		led_set|= 1<<i;
		LED_Ctl(led_set);
		delay_ms(50);
	}
	
	for(i=0; i<8; i++)
	{
		Channel_Select(i);
		delay_ms(50);
		
		plase = 0;
		flag = 0;
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //允许捕获中断
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //允许更新中断
		TIM_Cmd(TIM3, ENABLE); //开定时器
		
		while(!flag);
		
		for(total=0, j=(ARG*0.25); j<(ARG*0.75); j++)
		{
			total += arv[j];
		}
		
		if(total > ADJ_DATA_MAX || total < ADJ_DATA_MIN) 
		{//校准结果不合理，直接读取之前校准数据
				for(i=0; i<4; i++)
				{//同步快闪3次
						LED_Ctl(0xff);
						delay_ms(200);
						LED_Ctl(0x00);
						delay_ms(200);
				}
				break;
		}
		
		
		data[i] = total * 1.03;
	}
	
	if(i == 8)
	{//校准成功，保存数据
		Write_Flash(data);
		LED_Ctl(0x00);
		delay_ms(200);
		LED_Ctl(0xff);
		delay_ms(200);
		Read_Flash(thresholds);
	}
	
	LED_Ctl(0x00);
}

static void Read_Flash(uint32_t thresholds[])
{
	int i;
	uint32_t *p = (uint32_t *)(0x08000000 | (0x400*15));
	
	for(i=0; i<8; i++)
	{
		thresholds[i] = p[i];
	}
}


/*************************END main.c***************************/


