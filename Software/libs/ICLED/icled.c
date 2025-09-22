#include "icled.h"
#include "platformGlue.h"
#include <string.h>


/* WE ICLED */
#define BITS_PER_ICLED 	24

#define HIGH 			40-0 // 40-2 for AZ-Delivery
#define LOW 			20-0 // 20-2 for AZ-Delivery
#define RESET 			0

#define START_OFFSET 	85
#define RESET_OFFSET 	85

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
