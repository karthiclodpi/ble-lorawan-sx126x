/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    timer_template.h
  * @author  MCD Application Team
  * @brief   Timer abstraction layer header
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_TEMPLATE_H__
#define __TIMER_TEMPLATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32wb0x.h"
#include "stm32wb0x_hal_radio_timer.h"

#define TIMERTIME_T_MAX                             ( ( uint32_t )~0 )

/* Exported types ------------------------------------------------------------*/
/**
  * @brief Timer object description
  */
typedef struct {
    VTIMER_HandleType handle;  /**< HAL_RADIO_TIMER virtual timer handle */
    uint32_t timeout;          /**< Timer timeout in ms */
} TimerEvent_t;

/**
  * @brief Timer time representation in milliseconds
  */
typedef uint32_t TimerTime_t;

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Create a timer object
 * @param HANDLE: Pointer to the timer object
 * @param CB: Callback function
 */
void TimerInit(TimerEvent_t *HANDLE, VTIMER_CallbackType CB);

/**
 * @brief Set the timer period
 * @param HANDLE: Pointer to the timer object
 * @param TIMEOUT: Timeout value in milliseconds
 */
void TimerSetValue(TimerEvent_t *HANDLE, TimerTime_t TIMEOUT);

/**
 * @brief Start the timer
 * @param HANDLE: Pointer to the timer object
 */
void TimerStart(TimerEvent_t *HANDLE);

/**
 * @brief Stop the timer
 * @param HANDLE: Pointer to the timer object
 */
void TimerStop(TimerEvent_t *HANDLE);

/**
 * @brief Get the current system time in milliseconds
 * @return Current time in milliseconds
 */
TimerTime_t TimerGetCurrentTime(void);

/**
 * @brief Get the elapsed time between a reference and the current time
 * @param past: Reference time in milliseconds
 * @return Elapsed time in milliseconds
 */
TimerTime_t TimerGetElapsedTime(TimerTime_t past);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_TEMPLATE_H__ */
