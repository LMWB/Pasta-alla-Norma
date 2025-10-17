#include "platformGlue.h"
#include "ICLED/icled.h"
#include "ICLED/icled_config.h"

#include <stdlib.h>   // for rand()

typedef struct __rgb_fsm{
	pixel_t* color_start;
	pixel_t* color_stop;
	uint8_t next_state[2];
	void (*callback)(pixel_t* color_start, pixel_t* color_stop, uint8_t fade_progress);
}rgb_fsm_t;

#define BRIGHTNESS 255
#define delay_ms 50

pixel_t gamma_corrected;
pixel_t black =	{ .R = 0, .G = 0, .B = 0 };
pixel_t white =	{ .R = 255, .G = 255, .B = 255 };

void fade_colors( pixel_t* color_start, pixel_t* color_stop, uint8_t fade_progress)
{
	blend_and_dim_to_linear(color_start, color_stop, fade_progress, BRIGHTNESS, &gamma_corrected);
}

void rgb_ball_all_on(void){
//	icled_set_color(&white, 0);
//	icled_set_color(&white, 1);
//
//	icled_set_color(&black, 2);
//
//	icled_set_color(&white, 3);
//	icled_set_color(&white, 4);
//
//	icled_set_color(&black, 5);
//
//	icled_set_color(&white, 6);
//	icled_set_color(&white, 7);
//
//	icled_set_color(&black, 8);
//
//	icled_set_color(&white, 9);
//	icled_set_color(&white, 10);
//
//	icled_set_color(&black, 11);

	for (uint_fast8_t j = 0; j < MAX_NO_OF_LEDS; j++) {
		icled_set_color(&white, j);
	}
	icled_write_pixel_buffer_to_pwm();
}

void rgb_ball_all_off(void){
	for (uint_fast8_t j = 0; j < MAX_NO_OF_LEDS; j++) {
		icled_set_color(&black, j);
	}
	icled_write_pixel_buffer_to_pwm();
}

void send_to_led(pixel_t* color){
	/* copy one pixel to specific positions only and fire the pwm  */
//	icled_set_color(color, 0);
//	icled_set_color(color, 1);
//	icled_set_color(&black, 2);
//	icled_set_color(color, 3);
//	icled_set_color(color, 4);
//	icled_set_color(&black, 5);
//	icled_set_color(color, 6);
//	icled_set_color(color, 7);
//	icled_set_color(&black, 8);
//	icled_set_color(color, 9);
//	icled_set_color(color, 10);
//	icled_set_color(&black, 11);

	for (uint_fast8_t j = 0; j < MAX_NO_OF_LEDS; j++) {
		icled_set_color(color, j);
	}
	icled_write_pixel_buffer_to_pwm();
}

void rgb_ball_init(void){
	// pull output 1 low to have something to read from IN1 and IN2
	WRITE_PIN(OUT1_GPIO_Port, OUT1_Pin, 0);
	rgb_ball_all_off();
	DELAY(200);
}

void rgb_ball_blink(uint8_t how_often){
	for (uint8_t i = 0; i < how_often; i++) {
		rgb_ball_all_on();
		DELAY(500);

		rgb_ball_all_off();
		DELAY(500);
	}
}

uint8_t rgb_ball_read_jumpers(void){

	uint8_t jumper_state = 0;
	// read from pin IN1 and IN2
	jumper_state |= (READ_PIN(IN1_GPIO_Port, IN1_Pin) << 0);
	jumper_state |= (READ_PIN(IN2_GPIO_Port, IN2_Pin) << 1);
	return jumper_state;
}

void rgb_ball_rainbow_fade(void) {
	uint8_t i = 1;
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

		send_to_led(&gamma_corrected);
		LED1_TOGGLE();
		DELAY(delay_ms);
	}
}

void rgb_ball_christmas_color_fade(void) {
	uint8_t i = 1;

	// 1. Traditional Christmas, Red → Green → Gold → White → back to Red (gives a smooth and elegant looping sequence)
	pixel_t Deep_Red = 		{190, 0, 0};	// Warm, deep red (like ornaments)
	pixel_t Emerald_Green = {0, 128, 0};	// Classic pine green
	pixel_t Gold = 			{255, 215, 0};	// Warm golden accent
	pixel_t White = 		{255, 255, 180};// Slightly warm white (like fairy lights)

	// 2. Candy Cane Theme Red → White → Mint → White → Red (simple, vivid, “peppermint swirl” vibe)
	pixel_t Bright_Red = {255, 0, 0};	 // Candy red
	pixel_t Bright_White = {255, 255, 255};//	Bright white
	pixel_t Mint_Green = {0, 255, 150};  // Fresh minty accent

	// 3. Cozy Fireplace Theme Red → Orange → Amber → White → Red (simulates flickering warmth, great if you add brightness variation)
	pixel_t Crimson	= 	{180, 20, 30};
	pixel_t Orange	= 	{255, 80, 0};
	pixel_t Amber = 	{255, 160, 30};
	pixel_t Warm_White = {255, 220, 180};

	// 4. Frosty Winter Theme Blue → White → Lavender → Silver → Blue (cool and elegant, “frozen forest” mood)
	pixel_t Ice_Blue	= {100, 180, 255};
	pixel_t Snow_White	= {240, 255, 255};
	pixel_t Lavender	= {180, 160, 255};
	pixel_t Silver		= {200, 200, 200};

	pixel_t black		= {0, 0, 0};

	uint8_t z0 = 0;
	uint8_t z1 = 0;
	uint8_t input_vector = 0;
	rgb_fsm_t FSM[] = {
			{&Deep_Red,		&Emerald_Green, {255, 1}, fade_colors}, // 255 = stay, !255 = new state
			{&Emerald_Green,&Gold, 			{255, 2}, fade_colors},
			{&Gold,			&White, 		{255, 3}, fade_colors},
			{&White,		&Deep_Red, 		{255, 0}, fade_colors},
	};

//	rgb_fsm_t FSM[] = {
//			{&Bright_Red,	&Bright_White,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
//			{&Bright_White,	&Mint_Green, 	{255, 2}, fade_colors},
//			{&Mint_Green,	&Bright_Red, 	{255, 0}, fade_colors},
//	};

//	rgb_fsm_t FSM[] = {
//			{&Crimson,		&Orange,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
//			{&Orange,		&Amber, 	{255, 2}, fade_colors},
//			{&Amber,		&Warm_White,{255, 3}, fade_colors},
//			{&Warm_White,	&Crimson, 	{255, 0}, fade_colors},
//	};

//	rgb_fsm_t FSM[] = {
//			{&Ice_Blue,		&Snow_White,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
//			{&Snow_White,	&Lavender, 	{255, 2}, fade_colors},
//			{&Lavender,		&Silver,{255, 3}, fade_colors},
//			{&Silver,		&Ice_Blue, 	{255, 0}, fade_colors},
//	};

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

		send_to_led(&gamma_corrected);
		LED1_TOGGLE();
		DELAY(delay_ms);
	}
}

// sunstorm palette
static pixel_t sunstorm_colors[] = {
    {120,  40,   0},   // calm base
    {200,  60,   0},   // rising heat
    {255, 100,   0},   // burst
    {255, 180,  60},   // flare peak
    {255, 241, 224},   // plasma flash
    {180,  40,   0},   // cooling
};

void fade_between(const pixel_t* a, const pixel_t* b, uint8_t steps, uint16_t base_delay_ms)
{
    for (uint8_t i = 0; i <= steps; i++) {
        // Linear interpolation (integer)
        uint8_t R = a->R + ((int16_t)(b->R - a->R) * i) / steps;
        uint8_t G = a->G + ((int16_t)(b->G - a->G) * i) / steps;
        uint8_t B = a->B + ((int16_t)(b->B - a->B) * i) / steps;
        pixel_t mixed = {R, G, B};

        // random flicker brightness: around 80–100%
        //uint8_t flicker = 200 + (rand() % 56);  // 200–255
        uint8_t flicker = 180 + (rand() % 76);  // 180–255


        apply_gamma_and_brightness(&mixed, flicker, &gamma_corrected);
        send_to_led(&gamma_corrected);
        LED1_TOGGLE();

        // random delay variation (±30%)
        uint16_t flicker_delay = base_delay_ms + (rand() % (base_delay_ms / 2)) - (base_delay_ms / 4);
        DELAY(flicker_delay);
    }
}

void sunstorm_cycle(void)
{
	uint8_t NUM_COLORS = (sizeof(sunstorm_colors)/sizeof(sunstorm_colors[0]));
    while (1) {
        for (uint8_t i = 0; i < NUM_COLORS - 1; i++) {
            // faster in hot phases, slower in cool ones

            //uint8_t steps = (i == 2 || i == 3) ? 25 : 40;
            //uint16_t delay = (i == 2 || i == 3) ? 15 : 40;

            uint8_t steps 	= (i == 3 || i == 4) ? 40 : 100;
            uint16_t delay 	= (i == 3 || i == 4) ? 20 : 100;

            pixel_t* a = &sunstorm_colors[i];
            pixel_t* b = &sunstorm_colors[ (i+1) % NUM_COLORS];
            fade_between(a, b, steps, delay);
        }
    }
}


void rgb_ball_sun_storm(void){
	//
	pixel_t Calm_Base	= {120, 40,		0}; 	// dim orange glow (solar surface)
	pixel_t Rising_Heat	= {200, 60, 	0}; 	// deeper red-orange
	pixel_t Burst_Core  = {255, 100, 	0}; 	// bright red with some yellow
	pixel_t Flare_Peak	= {255, 180, 	60}; 	// glowing orange-white
	pixel_t Plasma_Flash= {255, 240,	200}; 	// almost white-hot
	pixel_t Cooling_Down= {180, 40,		0}; 	// back to dark red-orange

	pixel_t black = {0, 0, 0};

	uint8_t i = 1;
	uint8_t z0 = 0;
	uint8_t z1 = 0;
	uint8_t input_vector = 0;

	rgb_fsm_t FSM[] = {
			{&Calm_Base,	&Rising_Heat,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
			{&Rising_Heat,	&Burst_Core,	{255, 2}, fade_colors},
			{&Burst_Core,	&Flare_Peak, 	{255, 3}, fade_colors},
			{&Flare_Peak,	&Plasma_Flash, 	{255, 4}, fade_colors},
			{&Plasma_Flash,	&Cooling_Down, 	{255, 5}, fade_colors},
			{&Cooling_Down,	&Calm_Base, 	{255, 0}, fade_colors},
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

		send_to_led(&gamma_corrected);
		LED1_TOGGLE();
		DELAY(delay_ms);
	}
}

void rgb_ball_daylight(void){

	// ambient daylight cycle, simulate a full day’s light — from sunrise → noon → sunset → night → and back to sunrise
	// by smoothly fading through different color temperatures.
	pixel_t Dawn = 		{255, 140, 60}; 	// Soft orange glow (sunrise) 2000 K
	pixel_t Morning = 	{255, 200, 120}; 	// Warm white, still golden 3500 K
	pixel_t Noon = 		{255, 255, 255};	// Neutral, bright daylight 5500 K
	pixel_t Sunset = 	{255, 160, 80};		// Warm amber light 3000 K
	pixel_t Night = 	{80, 50, 20};		// Very dim warm glow (candlelight) 1500 K

	// What “Daylight” Means in RGB
	pixel_t Warm_White = 		{255, 214, 170}; // Cozy, like incandescent bulb, ~2700 K
	pixel_t Neutral_White = 	{255, 241, 224}; // Balanced, soft office light, ~4000 K
	pixel_t Daylight_White = 	{255, 255, 255}; // (pure white) or {255, 250, 244} Midday sunlight look, ~5500–6500 K
	pixel_t Cool_White = 		{220, 235, 255}; // Slightly bluish, like overcast sky, ~7500 K

	pixel_t black = {0, 0, 0};

	uint8_t i = 1;
	uint8_t z0 = 0;
	uint8_t z1 = 0;
	uint8_t input_vector = 0;
//	rgb_fsm_t FSM[] = {
//			{&Dawn,		&Morning,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
//			{&Morning,	&Noon, 		{255, 2}, fade_colors},
//			{&Noon,		&Sunset, 	{255, 3}, fade_colors},
//			{&Sunset,	&Night, 	{255, 0}, fade_colors},
//	};

	rgb_fsm_t FSM[] = {
			{&Warm_White,		&Neutral_White,	{255, 1}, fade_colors}, // 255 = stay, !255 = new state
			{&Neutral_White,	&Daylight_White,{255, 2}, fade_colors},
			{&Daylight_White,	&Cool_White, 	{255, 3}, fade_colors},
			{&Cool_White,		&Warm_White, 	{255, 0}, fade_colors},
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

		send_to_led(&gamma_corrected);
		LED1_TOGGLE();
		DELAY(delay_ms);
	}
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
			icled_write_pixel_buffer_to_pwm();
			LED1_TOGGLE();
		} else {
			send_to_led(&gamma_corrected);
			LED1_TOGGLE();
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
