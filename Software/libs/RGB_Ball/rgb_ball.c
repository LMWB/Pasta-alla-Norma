#include "platformGlue.h"
#include "ICLED/icled.h"
#include "ICLED/icled_config.h"

#define brightness 255
#define delay_ms 200

void rgb_ball_main(void) {
	uint8_t i = 0;
	uint8_t z = 0;

	pixel_t black = { .R = 0, .G = 0, .B = 0 };
	pixel_t cadetBlue = { .R = 95, .G = 158, .B = 160 };
	pixel_t darkorange = { .R = 255, .G = 140, .B = 0 };
	pixel_t crimson = { .R = 220, .G = 20, .B = 60 };
	pixel_t indigo = { .R = 75, .G = 0, .B = 130 };
	pixel_t olivedrab = { .R = 107, .G = 142, .B = 35 };
	pixel_t gamma_corrected;

	while (1) {

		switch (z) {
		case 0:
			blend_and_dim_to_linear(&cadetBlue, &darkorange, i++, brightness,
					&gamma_corrected);
			if (i == 255) {
				z = 1;
				i = 0;
			}
			break;
		case 1:
			blend_and_dim_to_linear(&darkorange, &crimson, i++, brightness,
					&gamma_corrected);
			if (i == 255) {
				z = 2;
				i = 0;
			}
			break;
		case 2:
			blend_and_dim_to_linear(&crimson, &indigo, i++, brightness,
					&gamma_corrected);
			if (i == 255) {
				z = 3;
				i = 0;
			}
			break;
		case 3:
			blend_and_dim_to_linear(&indigo, &olivedrab, i++, brightness,
					&gamma_corrected);
			if (i == 255) {
				z = 4;
				i = 0;
			}
			break;
		case 4:
			blend_and_dim_to_linear(&olivedrab, &cadetBlue, i++, brightness,
					&gamma_corrected);
			if (i == 255) {
				z = 0;
				i = 0;
			}
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
			/* copy one pixel to specific positions only and fire the pwm  */
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
		DELAY(delay_ms);
	}
}

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
