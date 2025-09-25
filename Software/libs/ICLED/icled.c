#include "icled.h"
#include "platformGlue.h"
#include <string.h>


/* WE ICLED */
#define BITS_PER_ICLED 	24

#define HIGH 			40-2 // 40-2 for AZ-Delivery, 40-0 for WE
#define LOW 			20-2 // 20-2 for AZ-Delivery, 20-0 for WE
#define RESET 			0

#define START_OFFSET 	85	//85
#define RESET_OFFSET 	85	//85

#define PWM_FRAME_SIZE (BITS_PER_ICLED * MAX_NO_OF_LEDS + START_OFFSET + RESET_OFFSET)

static pixel_t single_wire_LED_buffer[MAX_NO_OF_LEDS];

// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) -> Interrupt is placed in rollenralle.c


void icled_send_bit_stream(uint32_t *data_array, uint16_t length) {
	TIMER_COUNTER_START_DMA(data_array, length);
	DELAY(10);
}

// brightness: 0 (off) to 255 (full brightness)
uint8_t icled_apply_brightness(uint16_t position, uint8_t brightness) {
	if (position > MAX_NO_OF_LEDS) {
		return 0;
	} else{
		single_wire_LED_buffer[position].B = (uint8_t)((single_wire_LED_buffer[position].B * (uint16_t)brightness) / 255);
		single_wire_LED_buffer[position].G = (uint8_t)((single_wire_LED_buffer[position].G * (uint16_t)brightness) / 255);
		single_wire_LED_buffer[position].R = (uint8_t)((single_wire_LED_buffer[position].R * (uint16_t)brightness) / 255);
    return 1;
	}
}

#include <math.h>
#define GAMMA 2.2
static uint8_t gamma_table[256];

void init_gamma_table(void) {
    for (int i = 0; i < 256; i++) {
        gamma_table[i] = (uint8_t)(pow(i / 255.0, GAMMA) * 255.0 + 0.5);
    }
}

// brightness: 0–255
void apply_gamma_and_brightness(pixel_t* base_color, uint8_t brightness, pixel_t* result_color) {
    result_color->R = (uint8_t)((gamma_table[base_color->R] * (uint16_t)brightness) / 255);
    result_color->G = (uint8_t)((gamma_table[base_color->G] * (uint16_t)brightness) / 255);
    result_color->B = (uint8_t)((gamma_table[base_color->B] * (uint16_t)brightness) / 255);
}

pixel_t apply_gamma_and_brightness_x(pixel_t base_color, uint8_t brightness){
    pixel_t result_color;
	result_color.R = (uint8_t)((gamma_table[base_color.R] * (uint16_t)brightness) / 255);
    result_color.G = (uint8_t)((gamma_table[base_color.G] * (uint16_t)brightness) / 255);
    result_color.B = (uint8_t)((gamma_table[base_color.B] * (uint16_t)brightness) / 255);
    return result_color;
}

void gamma_demo(void) {
	init_gamma_table();

	pixel_t base = { 120, 50, 100 };
	pixel_t gamma_corrected;
	uint8_t brightness = 64; // ~25%

	apply_gamma_and_brightness(&base, brightness, &gamma_corrected);
}

uint8_t icled_set_color(pixel_t* pixel, uint16_t position){
	if (position > MAX_NO_OF_LEDS) {
		return 0;
	} else {
		single_wire_LED_buffer[position].B = pixel->B;
		single_wire_LED_buffer[position].G = pixel->G;
		single_wire_LED_buffer[position].R = pixel->R;
		return 1;
	}
}

// convert RGB Color code expressed as uint8 number to bit-stream zero and ones
void icled_convert_RGB_to_bit_code(pixel_t pixel, uint16_t *bit_buffer) {
	// swap bits since we need MSB....LSB
	// Separating color green
	for (uint32_t i = 0; i < 8; i++) {
		if (pixel.G & (1 << i)) {
			bit_buffer[7 - i] = 1;
		} else {
			bit_buffer[7 - i] = 0;
		}
	}
	// Separating color red
	for (uint32_t i = 0; i < 8; i++) {
		if (pixel.R & (1 << i)) {
			bit_buffer[7 - i + 8] = 1;
		} else {
			bit_buffer[7 - i + 8] = 0;
		}
	}
	// Separating color blue
	for (uint32_t i = 0; i < 8; i++) {
		if (pixel.B & (1 << i)) {
			bit_buffer[7 - i + 16] = 1;
		} else {
			bit_buffer[7 - i + 16] = 0;
		}
	}
}

void icled_convert_bool_to_pwm_frame(uint16_t *bit_buffer, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		if (bit_buffer[i] == 1)
			bit_buffer[i] = HIGH;
		else {
			bit_buffer[i] = LOW;
		}
	}
}

static uint16_t bit_buffer[BITS_PER_ICLED] = { 0 };		// working buffer to convert RGB-Color to ICLED Bit Frame
static uint16_t pwm_dma_buffer[PWM_FRAME_SIZE] = { 0 }; 	// buffer to send to ICLED

uint8_t icled_write_pixel_buffer_to_pwm(void){
	/* convert one RGB-coler to bit pattern and place it in the bit buffer that holds the total */
	for(uint_fast16_t k = 0; k < MAX_NO_OF_LEDS; ++k)
	{
		icled_convert_RGB_to_bit_code(single_wire_LED_buffer[k], bit_buffer);
		icled_convert_bool_to_pwm_frame(bit_buffer, BITS_PER_ICLED);
		memcpy(&pwm_dma_buffer[RESET_OFFSET + k * BITS_PER_ICLED], bit_buffer, BITS_PER_ICLED * 2);
	}
	icled_send_bit_stream((uint32_t*) pwm_dma_buffer, PWM_FRAME_SIZE);
	DELAY(10);
	return 1;
}

uint8_t icled_write_n_pixel_buffer_to_pwm(uint8_t no_of_leds){
	/* convert one RGB-coler to bit pattern and place it in the bit buffer that holds the total */
	for(uint_fast16_t k = 0; k < no_of_leds; ++k)
	{
		icled_convert_RGB_to_bit_code(single_wire_LED_buffer[k], bit_buffer);
		icled_convert_bool_to_pwm_frame(bit_buffer, BITS_PER_ICLED);
		memcpy(&pwm_dma_buffer[RESET_OFFSET + k * BITS_PER_ICLED], bit_buffer, BITS_PER_ICLED * 2);
	}
	uint16_t size = (BITS_PER_ICLED * no_of_leds) + START_OFFSET + RESET_OFFSET;
	icled_send_bit_stream((uint32_t*) pwm_dma_buffer, PWM_FRAME_SIZE);
	DELAY(10);
	return 1;
}
