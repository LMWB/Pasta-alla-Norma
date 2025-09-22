/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Onboard_LED_Pin GPIO_PIN_9
#define Onboard_LED_GPIO_Port GPIOB
#define DI1_Pin GPIO_PIN_2
#define DI1_GPIO_Port GPIOB
#define DI3_Pin GPIO_PIN_9
#define DI3_GPIO_Port GPIOA
#define DI4_Pin GPIO_PIN_6
#define DI4_GPIO_Port GPIOC
#define DI5_Pin GPIO_PIN_10
#define DI5_GPIO_Port GPIOA
#define DI6_Pin GPIO_PIN_11
#define DI6_GPIO_Port GPIOA
#define DI7_Pin GPIO_PIN_12
#define DI7_GPIO_Port GPIOA
#define DI8_Pin GPIO_PIN_15
#define DI8_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
