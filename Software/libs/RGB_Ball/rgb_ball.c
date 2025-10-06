#include "platformGlue.h"
#include "ICLED/icled.h"
#include "ICLED/icled_config.h"

typedef struct __rgb_fsm{
	pixel_t* color_start;
	pixel_t* color_stop;
	uint8_t next_state[2];
	void (*callback)(pixel_t* color_start, pixel_t* color_stop, uint8_t fade_progress);
}rgb_fsm_t;

#define BRIGHTNESS 255
#define delay_ms 50

pixel_t gamma_corrected;

void fade_colors( pixel_t* color_start, pixel_t* color_stop, uint8_t fade_progress)
{
	blend_and_dim_to_linear(color_start, color_stop, fade_progress, BRIGHTNESS, &gamma_corrected);
}

void rgb_ball_rainbow_fade(void) {
	uint8_t i = 1;
	pixel_t black =		{ .R = 0, 	.G = 0,		.B = 0 };
	pixel_t red = 		{ .R = 255, .G = 0,		.B = 0 };
	pixel_t orange =	{ .R = 255, .G = 165,	.B = 0 };
	pixel_t yellow = 	{ .R = 255, .G = 255,	.B = 0 };
	pixel_t green = 	{ .R = 0,	.G = 128,	.B = 0 };
	pixel_t blue = 		{ .R = 0, 	.G = 0,		.B = 255 };
	pixel_t indigo =	{ .R = 75, 	.G = 0, 	.B = 130 };
	pixel_t violet = 	{ .R = 238, .G = 130,	.B = 238 };

	uint8_t z0 = 0;
	uint8_t z1 = 0;
	uint8_t input_vector = 0;
	rgb_fsm_t FSM[] = {
	{ &red, 	&orange, 	{ 255, 1 }, fade_colors }, // 255 = stay, !255 = new state
	{ &orange, 	&yellow, 	{ 255, 2 }, fade_colors },
	{ &yellow, 	&green,		{ 255, 3 }, fade_colors },
	{ &green, 	&blue,		{ 255, 4 }, fade_colors },
	{ &blue,	&indigo, 	{ 255, 5 }, fade_colors },
	{ &indigo,	&violet, 	{ 255, 6 }, fade_colors },
	{ &violet,	&red, 		{ 255, 0 }, fade_colors }};

	while (1) {
		if (i == 0) {
			input_vector = 1;
		} else {
			input_vector = 0;
		}

		// read new state according to progress iteration
		z1 = FSM[z0].next_state[input_vector];

		// switch to next state if not STAY
		if (z1 != 255) {
			z0 = z1;
		}

		// execute function belonging to the new state
		FSM[z0].callback(FSM[z0].color_start, FSM[z0].color_stop, i++);

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
		DELAY(delay_ms);
	}
}

void rgb_ball_christmas_color_fade(void) {
	uint8_t i = 1;
	pixel_t black = {.R = 0,	.G = 0, 	.B = 0 };
	pixel_t red1 = { .R = 84,	.G = 8,		.B = 4 };
	pixel_t red2 = { .R = 129,	.G = 23,	.B = 27 };
	pixel_t red3 = { .R = 173,	.G = 46,	.B = 36 };
	pixel_t red4 = { .R = 199,	.G = 81,	.B = 70 };
	pixel_t red5 = { .R = 234,	.G = 140,	.B = 85 };
	pixel_t magenta = { .R = 255,	.G = 0,	.B = 255 };

	uint8_t z0 = 0;
	uint8_t z1 = 0;
	uint8_t input_vector = 0;
	rgb_fsm_t FSM[] = {
			{&red1,		&red2, {255, 1}, fade_colors}, // 255 = stay, !255 = new state
			{&red2,		&red3, {255, 2}, fade_colors},
			{&red3,		&red4, {255, 3}, fade_colors},
			{&red4,		&red5, {255, 4}, fade_colors},
			{&red5,		&magenta, {255, 5}, fade_colors},
			{&magenta,	&red1, {255, 0}, fade_colors},
	};

	while(1){
		if(i == 0){
			input_vector = 1;
		}else{
			input_vector = 0;
		}

		// read new state according to progress iteration
		z1 = FSM[z0].next_state[ input_vector ];

		// switch to next state if not STAY
		if(z1 != 255){ z0 = z1; }

		// execute function belonging to the new state
		FSM[z0].callback( FSM[z0].color_start, FSM[z0].color_stop, i++);

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
		DELAY(delay_ms);
	}



//	uint8_t z = 0;
//	while (1) {
//		switch (z) {
//		case 0:
//			blend_and_dim_to_linear(&red1, &red2, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 1;
//				i = 0;
//			}
//			break;
//		case 1:
//			blend_and_dim_to_linear(&red2, &red3, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 2;
//				i = 0;
//			}
//			break;
//		case 2:
//			blend_and_dim_to_linear(&red3, &red4, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 3;
//				i = 0;
//			}
//			break;
//		case 3:
//			blend_and_dim_to_linear(&red4, &red5, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 4;
//				i = 0;
//			}
//			break;
//		case 4:
//			blend_and_dim_to_linear(&red5, &green, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 5;
//				i = 0;
//			}
//			break;
//		case 5:
//			blend_and_dim_to_linear(&green, &red1, i++, BRIGHTNESS, &gamma_corrected);
//			if (i == 255) {
//				z = 0;
//				i = 0;
//			}
//			break;
//		default:
//			break;
//		}
//
//		/* copy one pixel to specific positions only and fire the pwm  */
//		icled_set_color(&gamma_corrected, 0);
//		icled_set_color(&gamma_corrected, 1);
//		icled_set_color(&black, 2);
//		icled_set_color(&gamma_corrected, 3);
//		icled_set_color(&gamma_corrected, 4);
//		icled_set_color(&black, 5);
//		icled_set_color(&gamma_corrected, 6);
//		icled_set_color(&gamma_corrected, 7);
//		icled_set_color(&black, 8);
//		icled_set_color(&gamma_corrected, 9);
//		icled_set_color(&gamma_corrected, 10);
//		icled_set_color(&black, 11);
//		HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
//		icled_write_pixel_buffer_to_pwm();
//		DELAY(delay_ms);
//	}
}

void rgb_ball_main(void) {
	uint8_t i = 0;
	uint8_t z = 0;

	pixel_t black = { .R = 0, .G = 0, .B = 0 };
	pixel_t cadetBlue = { .R = 95, .G = 158, .B = 160 };
	pixel_t darkorange = { .R = 255, .G = 140, .B = 0 };
	pixel_t crimson = { .R = 220, .G = 20, .B = 60 };
	pixel_t indigo = { .R = 75, .G = 0, .B = 130 };
	pixel_t olivedrab = { .R = 107, .G = 142, .B = 35 };

	while (1) {

		switch (z) {
		case 0:
			blend_and_dim_to_linear(&cadetBlue, &darkorange, i++, BRIGHTNESS, &gamma_corrected);
			if (i == 255) {
				z = 1;
				i = 0;
			}
			break;
		case 1:
			blend_and_dim_to_linear(&darkorange, &crimson, i++, BRIGHTNESS,
					&gamma_corrected);
			if (i == 255) {
				z = 2;
				i = 0;
			}
			break;
		case 2:
			blend_and_dim_to_linear(&crimson, &indigo, i++, BRIGHTNESS,
					&gamma_corrected);
			if (i == 255) {
				z = 3;
				i = 0;
			}
			break;
		case 3:
			blend_and_dim_to_linear(&indigo, &olivedrab, i++, BRIGHTNESS,
					&gamma_corrected);
			if (i == 255) {
				z = 4;
				i = 0;
			}
			break;
		case 4:
			blend_and_dim_to_linear(&olivedrab, &cadetBlue, i++, BRIGHTNESS,
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
