/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32wb0x_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void lowPowerIOSetup(void);
/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */
  lowPowerIOSetup();

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */

/**
  * @brief  Configures the IOs pull resistors to have the optimized power consumption.
  * @param  None
  * @retval None
  */
static void lowPowerIOSetup(void)
{
  
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, 
                       PWR_GPIO_BIT_0|
                       PWR_GPIO_BIT_1|
                       PWR_GPIO_BIT_2|                         
                       PWR_GPIO_BIT_3);
  
  HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, 
                             PWR_GPIO_BIT_8|
                             PWR_GPIO_BIT_9|
                             PWR_GPIO_BIT_10|
                             PWR_GPIO_BIT_11);
  
  HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_B, 
                             PWR_GPIO_BIT_0|
                             PWR_GPIO_BIT_3|
                             PWR_GPIO_BIT_6|
                             PWR_GPIO_BIT_7|
                             PWR_GPIO_BIT_12|
                             PWR_GPIO_BIT_13);
  
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B, 
                       PWR_GPIO_BIT_1|
                       PWR_GPIO_BIT_2|
                       PWR_GPIO_BIT_4|
                       PWR_GPIO_BIT_5|  
                       PWR_GPIO_BIT_14|
                       PWR_GPIO_BIT_15);
}

/* USER CODE END 1 */
