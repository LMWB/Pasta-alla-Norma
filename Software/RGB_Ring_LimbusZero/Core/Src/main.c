/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "platformGlue.h"
#include "ICLED/icled.h"
#include "ICLED/icled_config.h"

#define brightness 255
#define delay_ms 200
  uint8_t i = 0;
  uint8_t z = 0;

  pixel_t black 	= {.R = 0, .G = 0, .B = 0 };
  pixel_t cadetBlue 	= {.R = 95, .G = 158, .B = 160 };
  pixel_t darkorange	= {.R = 255, .G = 140, .B = 0 };
  pixel_t crimson 	= {.R = 220, .G = 20, .B = 60 };
  pixel_t indigo 		= {.R = 75, .G = 0, .B = 130 };
  pixel_t olivedrab 	= {.R = 107, .G = 142, .B = 35};
  pixel_t gamma_corrected;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  init_gamma_table();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  switch (z) {
		case 0:
			blend_and_dim_to_linear(&cadetBlue, &darkorange, i++, brightness, &gamma_corrected);
			if(i == 255){ z = 1; i = 0;}
			break;
		case 1:
			blend_and_dim_to_linear(&darkorange, &crimson, i++, brightness, &gamma_corrected);
			if(i == 255){ z = 2; i = 0;}
			break;
		case 2:
			blend_and_dim_to_linear(&crimson, &indigo, i++, brightness, &gamma_corrected);
			if(i == 255){ z = 3; i = 0;}
			break;
		case 3:
			blend_and_dim_to_linear(&indigo, &olivedrab, i++, brightness, &gamma_corrected);
			if(i == 255){ z = 4; i = 0;}
			break;
		case 4:
			blend_and_dim_to_linear(&olivedrab, &cadetBlue, i++, brightness, &gamma_corrected);
			if(i == 255){ z = 0; i = 0;}
			break;

		default:
			break;
	}

		if (0) {
			/* copy one pixel to all LED present and fire the pwm */
			for (uint_fast8_t j = 0; j < MAX_NO_OF_LEDS; j++) {
				icled_set_color(&gamma_corrected, j);
			}
			HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
			icled_write_pixel_buffer_to_pwm();
		} else {
			/* write  one pixel to one specific position in the PWM buffer */
			icled_set_color(&gamma_corrected, 0);
			icled_set_color(&gamma_corrected, 1);
			icled_set_color(&black, 2);
			icled_set_color(&gamma_corrected, 3);
			icled_set_color(&gamma_corrected, 4);
			icled_set_color(&black, 5);
			icled_set_color(&gamma_corrected, 6);
			icled_set_color(&gamma_corrected, 7);
			icled_set_color(&black, 8);
			icled_set_color(&gamma_corrected, 9);
			icled_set_color(&gamma_corrected, 10);
			icled_set_color(&black, 11);
			HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
			icled_write_pixel_buffer_to_pwm();
		}

	/* this is the speed of fade progress */
	HAL_Delay(delay_ms);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIMER_COUNTER_INSTANCE) {
		/* on STM32 C0xx the DMA is stopping it self after Pulse is finished
		 * no need for stopping it manually just trigger a new pulse as soon
		 * the RGB-LEd color has changed */
//		TIMER_COUNTER_STOP_DMA();
//		TIMER_COUNTER_SET_DUTY_CYCLE(0);
		__NOP();
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
