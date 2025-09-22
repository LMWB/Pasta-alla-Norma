#ifndef HARDWARE_GLOBAL_H
#define HARDWARE_GLOBAL_H


#define PLATFORM_STM32C031

#define HAS_UART
#define HAS_TIMER
#define HAS_I2C
#define HAS_SPI
//#define HAS_WATCHDOG
//#define HAS_CAN
//#define PLATFORM_HAS_RTC


#include "main.h" // -> includes "stm32f4xx_hal.h" and all drivers
#include "gpio.h"

/* *** Platform delay (polling) ******************************************************** */
#define MAX_DELAY        HAL_MAX_DELAY
#define DELAY(ms)        HAL_Delay(ms)
#define GET_TICK()       HAL_GetTick()

/* *** Platform return codes ********************************************************** */
#define DEVICE_STATUS_DEFINITION        HAL_StatusTypeDef
#define DEVICE_OK                       HAL_OK
#define DEVICE_ERROR                    HAL_ERROR
#define DEVICE_BUSY                     HAL_BUSY
#define DEVICE_TIMEOUT                  HAL_TIMEOUT

/* *** GPIO *************************************************************************** */
#define LED1_ON() 					HAL_GPIO_WritePin(	LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF()					HAL_GPIO_WritePin(	LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_TOGGLE()				HAL_GPIO_TogglePin(	LED1_GPIO_Port, LED1_Pin)

#define LED2_ON() 					HAL_GPIO_WritePin(	LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF()					HAL_GPIO_WritePin(	LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_TOGGLE()				HAL_GPIO_TogglePin(	LED2_GPIO_Port, LED2_Pin)

#define LED3_ON() 					HAL_GPIO_WritePin(	LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF()					HAL_GPIO_WritePin(	LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED3_TOGGLE()				HAL_GPIO_TogglePin(	LED3_GPIO_Port, LED3_Pin)

#define TOGGLE_HEARTBEAT_LED()		LED1_TOGGLE()

#define TOGGLE_PIN(PORT, PIN)		HAL_GPIO_TogglePin(PORT, PIN)			/* void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) */
#define WRITE_PIN(PORT, PIN, STATE)	HAL_GPIO_WritePin(PORT, PIN, STATE); 	/* void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) */
#define READ_PIN(PORT, PIN)			HAL_GPIO_ReadPin(PORT, PIN)


/* *** UART *************************************************************************** */
#ifdef HAS_UART
#include "usart.h"
#define UART_TERMINAL_HANDLER 	            		huart2
#define UART_TERMINAL_INSTANCE 	            		USART2
#define UART_TERMINAL_SEND(string, size)    		HAL_UART_Transmit(&UART_TERMINAL_HANDLER, string, size, HAL_MAX_DELAY)

//#define UART_TERMINAL_READ_BYTE_IRQ(buffer)		HAL_UART_Receive_IT(&UART_TERMINAL_HANDLER, buffer, 1)
//#define UART_TERMINAL_READ_LINE_IRQ(buffer, size)	HAL_UARTEx_ReceiveToIdle_DMA(&UART_TERMINAL_HANDLER, buffer, size)
#define UART_START_RX_IT(pRXbuffer, size)			HAL_UARTEx_ReceiveToIdle_IT(&UART_TERMINAL_HANDLER, (uint8_t*)pRXbuffer, size);
#endif

/* *** I2C ********************************************************************************/
#ifdef HAS_I2C
#include "i2c.h"
#define I2C_HANDLER 				        		hi2c1
#define I2C_INSTANCE				        		I2C1
#define IS_I2C_DEVICE_READY(dev_address)    		HAL_I2C_IsDeviceReady(&I2C_HANDLER, dev_address, 1, 100)

#define I2C_READ_BYTE(dev_address, memory_adress, data, data_length) 	HAL_I2C_Mem_Read(&I2C_HANDLER, dev_address, memory_adress, 1, data, data_length, 0xFFFF);
#define I2C_WRITE_BYTE(dev_address, memory_adress, byte)				HAL_I2C_Mem_Write(&I2C_HANDLER, dev_address, memory_adress, 1, byte, 1, 0xFFFF);
#define I2C_WRITE_BYTES(dev_address, memory_adress, byte, length)		HAL_I2C_Mem_Write(&I2C_HANDLER, dev_address, memory_adress, 1, byte, length, 0xFFFF);

#endif

/* *** SPI ********************************************************************************/
#ifdef HAS_SPI
#include "spi.h"
#define SPI_HANDLER 										hspi1
#define SPI_INSTANCE										SPI1

//#define SPI_TRANSMIT_RECEIVE(tx_buffer, rxbuffer, length)	()
#define SPI_TRANSMIT(tx_buffer, length)						HAL_SPI_Transmit(&SPI_HANDLER, tx_buffer, length, 0xffff);

#define EEPROM_SPI_HANDLER 									hspi1
#define EEPROM_SPI_INSTANCE									SPI1
#define EEPROM_CHIP_SELECT()								HAL_GPIO_WritePin(EEPROM_nCS_GPIO_Port, EEPROM_nCS_Pin, 0)
#define EEPROM_CHIP_DESELECT()								HAL_GPIO_WritePin(EEPROM_nCS_GPIO_Port, EEPROM_nCS_Pin, 1)

#define EEPROM_SPI_TRANSMIT_BYTE(txData)					HAL_SPI_Transmit(&EEPROM_SPI_HANDLER, txData, 1, 0xff)
#define EEPROM_SPI_TRANSMIT_nBYTES(txData, length)			HAL_SPI_Transmit(&EEPROM_SPI_HANDLER, txData, length, 0xff)
#define EEPROM_SPI_RECEIVE_nBYTES(rxData, length)			HAL_SPI_Receive( &EEPROM_SPI_HANDLER, rxData, length, 0xff)
#endif

/* *** Hardware Timer *********************************************************************/

#ifdef HAS_TIMER
#include "tim.h"
#define TIMER_COUNTER_HANLDER						htim1
#define TIMER_COUNTER_INSTANCE						TIM1
#define TIMER_COUNTER_CHANNEL						TIM_CHANNEL_2
#define TIMER_COUNTER_SET_DUTY_CYCLE(uDutycyle)		__HAL_TIM_SET_COMPARE(&TIMER_COUNTER_HANLDER, TIMER_COUNTER_CHANNEL, uDutycyle)

#define TIMER_COUNTER_START_DMA(pPattern, uLength)	HAL_TIM_PWM_Start_DMA(		&TIMER_COUNTER_HANLDER, TIMER_COUNTER_CHANNEL, pPattern, uLength)
//#define TIMER_COUNTER_START_DMA(pPattern, uLength)	HAL_TIMEx_PWMN_Start_DMA(	&TIMER_COUNTER_HANLDER, TIMER_COUNTER_CHANNEL, pPattern, uLength)

#define TIMER_COUNTER_STOP_DMA()					HAL_TIM_PWM_Stop_DMA( 	&TIMER_COUNTER_HANLDER, TIMER_COUNTER_CHANNEL)
//#define TIMER_COUNTER_STOP_DMA()					HAL_TIMEx_PWMN_Stop_DMA(&TIMER_COUNTER_HANLDER, TIMER_COUNTER_CHANNEL)
#endif

/* *** Watch Dog **************************************************************************/
#ifdef HAS_WATCHDOG
#include "iwdg.h"
#define REFRESH_WATCHDOG()							HAL_IWDG_Refresh(&hiwdg);
#endif

/* *** Real Time Clock ********************************************************************/
#ifdef PLATFORM_HAS_RTC
#include "rtc.h"
#define RTC_HANDLER             hrtc
#define RTC_INSTANCE            RTC
#define SET_DEVICE_TIME(sTime)  HAL_RTC_SetTime( &RTC_HANDLER, sTime, RTC_FORMAT_BIN)
#define SET_DEVICE_DATE(sDate)  HAL_RTC_SetDate( &RTC_HANDLER, sDate, RTC_FORMAT_BIN)
#define GET_DEVICE_TIME(sTime)  HAL_RTC_GetTime( &RTC_HANDLER, sTime, RTC_FORMAT_BIN)
#define GET_DEVICE_DATE(sDate)  HAL_RTC_GetDate( &RTC_HANDLER, sDate, RTC_FORMAT_BIN)
#endif

/* *** CAN ********************************************************************/
#ifdef HAS_CAN
#include "can.h"
#define CAN_HANDLER 							hcan
#define CAN_INSTANCE            				CAN
#define CAN_CONFIGURE_FILTER(canFilterConfig)	HAL_CAN_ConfigFilter(&CAN_HANDLER, &canFilterConfig)
#define CAN_START_PERIPHERAL() 					HAL_CAN_Start(&CAN_HANDLER)
#define CAN_STOP_PERIPHERAL() 					HAL_CAN_Stop(&CAN_HANDLER)
#define CAN_ACTIVATE_RX_INTERRUPT() 			HAL_CAN_ActivateNotification(&CAN_HANDLER, CAN_IT_RX_FIFO0_MSG_PENDING)
static uint32_t  TxMailbox1;
#define SEND_CAN_MESSAGE(txHeader, txData) 		HAL_CAN_AddTxMessage(&CAN_HANDLER, &txHeader, txData, &TxMailbox1);
#endif

/* *** UID ********************************************************************/
typedef union {
	uint32_t uid[3];
	uint8_t uid_byte[12]; // 3 * uint32 = 3 * 4bytes = 12 uint8
} cpu_uid_t;

#define READ_CPU_UID(cpu_uid_type)	{cpu_uid_type.uid[0] = HAL_GetUIDw0(); cpu_uid_type.uid[1] = HAL_GetUIDw1(); cpu_uid_type.uid[2] = HAL_GetUIDw2();}

#endif /* HARDWARE_GLOBAL_H */
