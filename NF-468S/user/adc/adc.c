#include "adc.h"
#include "delay.h"

extern volatile uint16_t ADC1ConvertedValue[2];   //ADC数值

//ADC_IN1  PA1


static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef  PORT_ADC; 
	
	/* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* PA1 mode as analog */
	PORT_ADC.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1;
  PORT_ADC.GPIO_Mode  = GPIO_Mode_AN;
  PORT_ADC.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &PORT_ADC);
}

static void DMA_Config(void)
{
  DMA_InitTypeDef   DMA_InitStructure;
  
	/* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
	/* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE); 
}

void ADC_InitConfig(void)
{
	ADC_InitTypeDef ADC_InitStruct = {0};
	
	ADC_GPIO_Config();
	DMA_Config();
	/* ADC periph clock enable */
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_DeInit(ADC1);
	
	ADC_StructInit(&ADC_InitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;   //连续转换
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //转换结果右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;    //不用外部触发转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                //分辨率12位
	ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);
//	ADC_ClockModeConfig(ADC1, ADC_ClockMode_SynClkDiv4);

	ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_55_5Cycles); //转换时间
	ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_55_5Cycles);
	ADC_TempSensorCmd(ENABLE);
	
	/* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
	ADC_ClearFlag(ADC1,ADC_FLAG_ADRDY); // For HK32F0xx case
	
	/* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);

	/* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
	
	ADC_Cmd(ADC1, ENABLE); 
	
	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
	
	ADC_StartOfConversion(ADC1);
	
}	






