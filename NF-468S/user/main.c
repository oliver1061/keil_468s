#include "gpioconfig.h"
#include "time.h"
#include "select.h"
#include "delay.h"
#include "cd4017.h"
#include "adc.h"

//V1.1  ��У׼MIN��ֵ  ADJ_DATA_MIN �� 18900 ��Ϊ 16500
//V1.2  ��һ���ܲ��������ύ�޸ĺ������


//Schmitt
#define    THRESHOLD      50     //��ֵ
#define    QUAN           3      //����

#define ADJ_DATA_MAX        22100
#define ADJ_DATA_MIN        16500


typedef struct Schmitt
{
	uint16_t difference[QUAN];      //��ֵ
	uint8_t thre_qua[QUAN];         //�ﵽ��ֵ������
	uint8_t not_thre_qua[QUAN];     //�ﲻ����ֵ������
	uint8_t count_p;                //����ָ��
}schmitt;

schmitt buzzer = {0};

volatile uint32_t arv[ARG];        //����������
volatile uint16_t plase;           //������ָ��
volatile uint16_t updata_val;      //�����жϴ���
volatile uint8_t flag = 0;         //������ɱ�־
volatile uint16_t ADC1ConvertedValue[2];   //ADC��ֵ
volatile uint8_t voltage_low = 0;          //�͵�ѹ��־
uint8_t test_num = 9;                      //Զ�˼���

	
uint32_t total;
uint32_t thresholds[8];    //��ֵ

uint16_t set=0;

	//��ֵУ׼����
uint32_t data[8];

/**************************��������*********************************/
static void Write_Flash(uint32_t data[]);
static void adjusting(uint32_t data[]);
static void Read_Flash(uint32_t thresholds[]);
static void ADC_test(void);

/********************************************************************/



int main()
{
	int i, j;
	
	//��ʼ������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	
	Channel_Select_Init();
	GPIO_InitConfig();
	CD4017_Init();
	Time3();
	TIM14_Init();
	ADC_InitConfig();
	
	//adjusting(data);
	//Write_Flash(data);        //дflash
//	LED_Ctl(0x00);
//	delay_ms(200);
//	LED_Ctl(0xff);
//	delay_ms(200);
//	LED_Ctl(0x00);
	
						
	//����ֵ
	Read_Flash(thresholds);
	
#if 0
	GPIOA->ODR |= GPIO_Pin_15;    //�л�ѹ�߲���
	GPIOB->ODR &= ~GPIO_Pin_6;
	GPIOB->ODR &= ~GPIO_Pin_7;
	LED_POWER_ON;    //����
	
	Channel_Select(3);
	delay_ms(30);
	
	
	while(1)
	{
		plase = 0;
		flag = 0;
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //�������ж�
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //��������ж�
		TIM_Cmd(TIM3, ENABLE); //����ʱ��
		while(!flag);
		
		delay_ms(200);
	}
#endif
	
	while(1)
	{
		ADC_test();
	
		if(voltage_low)    //�����ѹ��
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)    //ѹ��
			{
				GPIOA->ODR |= GPIO_Pin_15;    //�л�ѹ�߲���
				GPIOB->ODR &= ~GPIO_Pin_6;
				GPIOB->ODR &= ~GPIO_Pin_7;
				
				if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)       //ѹ��У׼
				{
					delay_ms(500);
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)
					{
						adjusting(data);
						
						Write_Flash(data);        //дflash
						LED_Ctl(0x00);
						delay_ms(200);
						LED_Ctl(0xff);
						delay_ms(200);
						LED_Ctl(0x00);
						
						Read_Flash(thresholds);    //����ֵ
						
						while(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0);    //�ȴ��ɿ�
					}
				}
				
				
				set = 0;
				
				for(i=0; i<8; i++)
				{
					Channel_Select(i);
					delay_ms(30);
					
					plase = 0;
					flag = 0;
					
					TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //�������ж�
					TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //��������ж�
					TIM_Cmd(TIM3, ENABLE); //����ʱ��
					
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
			
			
			else        //Զ��
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
		
		else    //��ѹ����
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)    //ѹ��
			{
				GPIOA->ODR |= GPIO_Pin_15;    //�л�ѹ�߲���
				GPIOB->ODR &= ~GPIO_Pin_6;
				GPIOB->ODR &= ~GPIO_Pin_7;
				LED_POWER_ON;    //����
				
				if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)       //ѹ��У׼
				{
					delay_ms(500);
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0)
					{
						adjusting(data);
						
						while(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0);    //�ȴ��ɿ�
					}
				}
				
				
				set = 0;
				
				for(i=0; i<8; i++)
				{
					Channel_Select(i);
					delay_ms(30);
					
					plase = 0;
					flag = 0;
					
					TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //�������ж�
					TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //��������ж�
					TIM_Cmd(TIM3, ENABLE); //����ʱ��
					
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
			
			
			else        //Զ��
			{
				GPIOA->ODR &= ~GPIO_Pin_15;  //�л�Զ��ģʽ
				GPIOB->ODR |= GPIO_Pin_6;   //PB6��
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
				LED_POWER_OFF;     //�ص�
				delay_ms(250);
				CLK_L;
				delay_ms(1);
				CLK_H;
				delay_ms(1);
				LED_POWER_ON;    //����
				delay_ms(250);
				
			}
		}
	

	}
}

static void ADC_test(void)
{
	
	
	
		while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
		DMA_ClearFlag(DMA1_FLAG_TC1);
	
	
	
	if(ADC1ConvertedValue[0] < 1860)    //��ѹ����6.5 2135  1970
	{
		voltage_low = 1;
	}
		
	else    //��ѹ����6.8  2335  2076
	{
		voltage_low = 0;
	}
		
	
}

	//дflash����
static void Write_Flash(uint32_t data[])
{
	int i;
	
			//дflash
		//1.����
		FLASH_Unlock();
		//2.����
		FLASH_ErasePage(0x08000000 | (0x400*15));
		//3.д����
		for(i=0; i<8; i++)
		{
			FLASH_ProgramWord(0x08000000 | (0x400*15) + 4*i, data[i]);
		}
		
		//4.д������
		FLASH_Lock();
}

static void adjusting(uint32_t data[])
{
	int i,j;
	uint8_t led_set = 0;
	
	delay_ms(500);
	
	//У׼��ʾ
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
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);      //�������ж�
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //��������ж�
		TIM_Cmd(TIM3, ENABLE); //����ʱ��
		
		while(!flag);
		
		for(total=0, j=(ARG*0.25); j<(ARG*0.75); j++)
		{
			total += arv[j];
		}
		
		if(total > ADJ_DATA_MAX || total < ADJ_DATA_MIN) 
		{//У׼���������ֱ�Ӷ�ȡ֮ǰУ׼����
				for(i=0; i<4; i++)
				{//ͬ������3��
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
	{//У׼�ɹ�����������
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


