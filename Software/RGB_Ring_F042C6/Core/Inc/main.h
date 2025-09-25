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
#include "stm32f0xx_hal.h"

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
#define DI18_Pin GPIO_PIN_13
#define DI18_GPIO_Port GPIOC
#define DI16_Pin GPIO_PIN_0
#define DI16_GPIO_Port GPIOA
#define DI14_Pin GPIO_PIN_1
#define DI14_GPIO_Port GPIOA
#define DI12_Pin GPIO_PIN_2
#define DI12_GPIO_Port GPIOA
#define DI10_Pin GPIO_PIN_3
#define DI10_GPIO_Port GPIOA
#define DI8_Pin GPIO_PIN_4
#define DI8_GPIO_Port GPIOA
#define DI6_Pin GPIO_PIN_5
#define DI6_GPIO_Port GPIOA
#define DI4_Pin GPIO_PIN_6
#define DI4_GPIO_Port GPIOA
#define DI2_Pin GPIO_PIN_7
#define DI2_GPIO_Port GPIOA
#define DI1_Pin GPIO_PIN_0
#define DI1_GPIO_Port GPIOB
#define DI3_Pin GPIO_PIN_1
#define DI3_GPIO_Port GPIOB
#define DI5_Pin GPIO_PIN_2
#define DI5_GPIO_Port GPIOB
#define DI7_Pin GPIO_PIN_10
#define DI7_GPIO_Port GPIOB
#define DI9_Pin GPIO_PIN_11
#define DI9_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOB
#define LED_TX_Pin GPIO_PIN_14
#define LED_TX_GPIO_Port GPIOB
#define LED_RX_Pin GPIO_PIN_15
#define LED_RX_GPIO_Port GPIOB
#define LED_RX_EXTI_IRQn EXTI4_15_IRQn
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOA
#define EEPROM_nCS_Pin GPIO_PIN_15
#define EEPROM_nCS_GPIO_Port GPIOA
#define DI11_Pin GPIO_PIN_6
#define DI11_GPIO_Port GPIOB
#define DI13_Pin GPIO_PIN_7
#define DI13_GPIO_Port GPIOB
#define DI15_Pin GPIO_PIN_8
#define DI15_GPIO_Port GPIOB
#define DI17_Pin GPIO_PIN_9
#define DI17_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
