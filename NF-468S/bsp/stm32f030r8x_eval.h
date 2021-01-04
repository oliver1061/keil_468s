/**
  ******************************************************************************
  * @file    stm32f030r8x_eval.h
  * @author  
  * @version 
  * @date    22-July-2019
  * @brief   This file contains definitions for STM320518_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F030R8TX_EVAL_H
#define __STM32F030R8TX_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  LED1 = 0,
  LED2 = 1
} Led_TypeDef;

typedef enum 
{
  BUTTON_KEY1 = 0,
  BUTTON_KEY2 = 1,
  BUTTON_WAKEUP = 2,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{
  COM1 = 0
} COM_TypeDef;   

/** @defgroup STM320518_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM320518_EVAL board  
  */ 
#if !defined (USE_STM320518_EVAL)
 #define USE_STM320518_EVAL
#endif

/**
 * @brief Select the revision A or B(default) of the EVAL board used.  
 */
#if !defined (STM320518_EVAL_REVB)
 #define STM320518_EVAL_REVB
#endif

#if !defined (STM320518_EVAL_REVA)
 /* #define STM320518_EVAL_REVA */
#endif


/** @addtogroup STM320518_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_2
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOA
  
#define LED2_PIN                         GPIO_Pin_3
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOA
  
/**
  * @}
  */ 

/** @addtogroup STM320518_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          3

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                  GPIO_Pin_6
#define KEY1_BUTTON_GPIO_PORT            GPIOF
#define KEY1_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define KEY1_BUTTON_EXTI_LINE            EXTI_Line13
#define KEY1_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOC
#define KEY1_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource6
#define KEY1_BUTTON_EXTI_IRQn            EXTI4_15_IRQn 

/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                  GPIO_Pin_7
#define KEY2_BUTTON_GPIO_PORT            GPIOF
#define KEY2_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define KEY2_BUTTON_EXTI_LINE            EXTI_Line8
#define KEY2_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOB
#define KEY2_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource7
#define KEY2_BUTTON_EXTI_IRQn            EXTI4_15_IRQn

/**
 * @brief Wakeup push-button
 */
#define WAKEUP_BUTTON_PIN                GPIO_Pin_13
#define WAKEUP_BUTTON_GPIO_PORT          GPIOC
#define WAKEUP_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOC
#define WAKEUP_BUTTON_EXTI_LINE          EXTI_Line13
#define WAKEUP_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOC
#define WAKEUP_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource13
#define WAKEUP_BUTTON_EXTI_IRQn          EXTI4_15_IRQn


/**
  * @}
  */ 


/** @addtogroup STM320518_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1

#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_1
   
#define EVAL_COM1_IRQn                   USART1_IRQn

/**
  * @}
  */

/** @addtogroup STM320518_EVAL_LOW_LEVEL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       RCC_APB2Periph_SPI1

#define SD_SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SD_SPI_SCK_SOURCE                GPIO_PinSource13
#define SD_SPI_SCK_AF                    GPIO_AF_0

#define SD_SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MISO_SOURCE               GPIO_PinSource14
#define SD_SPI_MISO_AF                   GPIO_AF_0


#define SD_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PA.07 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource15
#define SD_SPI_MOSI_AF                   GPIO_AF_0

#define SD_CS_PIN                        GPIO_Pin_12                 /* PB.12 */
#define SD_CS_GPIO_PORT                  GPIOB                       /* GPIOB */
#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOB
#define SD_CS_SOURCE                     GPIO_PinSource12
#define SD_CS_AF                         GPIO_AF_0

#define SD_DETECT_PIN                    GPIO_Pin_5                  /* PF.5 */
#define SD_DETECT_EXTI_LINE              EXTI_Line15
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource5
#define SD_DETECT_GPIO_PORT              GPIOF                       /* GPIOF */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOF
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOF
#define SD_DETECT_EXTI_IRQn              EXTI4_15_IRQn

/**
  * @}
  */


/** @addtogroup STM320518_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C1

#define LM75_I2C_SCL_PIN                 GPIO_Pin_6                  /* PB.06 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SCL_SOURCE              GPIO_PinSource6
#define LM75_I2C_SCL_AF                  GPIO_AF_1

#define LM75_I2C_SDA_PIN                 GPIO_Pin_7                  /* PB.07 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SDA_SOURCE              GPIO_PinSource7
#define LM75_I2C_SDA_AF                  GPIO_AF_1

#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_5                  /* PB.05 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOB                       /* GPIOB */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_AHBPeriph_GPIOB
#define LM75_I2C_SMBUSALERT_SOURCE       GPIO_PinSource5
#define LM75_I2C_SMBUSALERT_AF           GPIO_AF_3

/**
  * @}
  */
   
/** @addtogroup STM320518_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
   
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_1

#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.07 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource7
#define sEE_I2C_SDA_AF                   GPIO_AF_1

/**
  * @}
  */

/** @defgroup STM320518_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void sFLASH_LowLevel_DeInit(void);
void sFLASH_LowLevel_Init(void);
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void); 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM320518_EVAL_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
