#include "icled.h"
#include "platformGlue.h"
#include <string.h>


/* WE ICLED */
#define BITS_PER_ICLED 	24

#define HIGH 			40-2 // 40-2 for AZ-Delivery, 40-0 for WE
#define LOW 			20-2 // 20-2 for AZ-Delivery, 20-0 for WE
#define RESET 			0

#define START_OFFSET 	85 //40-2	//85
#define RESET_OFFSET 	85 //20-2	//85

#define PWM_FRAME_SIZE (BITS_PER_ICLED * MAX_NO_OF_LEDS + START_OFFSET + RESET_OFFSET)
static pixel_t single_wire_LED_buffer[MAX_NO_OF_LEDS];

// gamma correction look up table created with python script (tools folder)
static uint8_t gamma_table[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6,
		6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15,
		15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 26,
		27, 28, 28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41, 42,
		43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
		62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84,
		85, 87, 88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 100, 102, 103, 105, 106, 107, 109,
		110, 111, 113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133,
		135, 137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161,
		163, 165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186, 188, 190, 192,
		194, 196, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 225,
		227, 229, 231, 234, 236, 238, 240, 242, 244, 246, 248, 251, 253, 255};

void icled_send_bit_stream(uint32_t *data_array, uint16_t length) {
	TIMER_COUNTER_START_DMA(data_array, length);
}

// brightness: 0 (off) to 255 (full brightness)
//uint8_t icled_apply_brightness(uint16_t position, uint8_t brightness) {
//	if (position > MAX_NO_OF_LEDS) {
//		return 0;
//	} else{
//		single_wire_LED_buffer[position].B = (uint8_t)((single_wire_LED_buffer[position].B * (uint16_t)brightness) / 255);
//		single_wire_LED_buffer[position].G = (uint8_t)((single_wire_LED_buffer[position].G * (uint16_t)brightness) / 255);
//		single_wire_LED_buffer[position].R = (uint8_t)((single_wire_LED_buffer[position].R * (uint16_t)brightness) / 255);
//    return 1;
//	}
//}

//// brightness: 0–255
//void apply_gamma_and_brightness(pixel_t* base_color, uint8_t brightness, pixel_t* result_color) {
//    result_color->R = (uint8_t)((gamma_table[base_color->R] * (uint16_t)brightness) / 255);
//    result_color->G = (uint8_t)((gamma_table[base_color->G] * (uint16_t)brightness) / 255);
//    result_color->B = (uint8_t)((gamma_table[base_color->B] * (uint16_t)brightness) / 255);
//}

// t: 0..255 blend fraction (0 = a, 255 = b)
// brightness: 0..255 brightness in linear space
// out is the linear values to send to the LED (0..255)
void blend_and_dim_to_linear(pixel_t *a, pixel_t *b, uint8_t t, uint8_t brightness, pixel_t *out)
{
    uint16_t r1 = gamma_table[a->R];
    uint16_t g1 = gamma_table[a->G];
    uint16_t b1 = gamma_table[a->B];

    uint16_t r2 = gamma_table[b->R];
    uint16_t g2 = gamma_table[b->G];
    uint16_t b2 = gamma_table[b->B];

    uint32_t r = ((uint32_t)(255 - t) * r1 + (uint32_t)t * r2 + 127) / 255;
    uint32_t g = ((uint32_t)(255 - t) * g1 + (uint32_t)t * g2 + 127) / 255;
    uint32_t bl= ((uint32_t)(255 - t) * b1 + (uint32_t)t * b2 + 127) / 255;

    // apply global brightness (in linear)
    r  = (r  * brightness + 127) / 255;
    g  = (g  * brightness + 127) / 255;
    bl = (bl * brightness + 127) / 255;

    // clamp (probably unnecessary but safe)
    out->R = (uint8_t)(r > 255 ? 255 : r);
    out->G = (uint8_t)(g > 255 ? 255 : g);
    out->B = (uint8_t)(bl > 255 ? 255 : bl);
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

uint8_t icled_write_pixel_buffer_to_pwm(void){
	uint16_t pwm_dma_buffer[PWM_FRAME_SIZE] = { 0 }; 	// buffer to send to ICLED
	uint16_t bit_buffer[BITS_PER_ICLED] = { 0 };		// working buffer to convert RGB-Color to ICLED Bit Frame

	/* convert one RGB-color to bit pattern and place it in the bit buffer that holds the total */
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

//uint8_t icled_write_n_pixel_buffer_to_pwm(uint8_t no_of_leds){
//	uint16_t pwm_dma_buffer[PWM_FRAME_SIZE] = { 0 }; 	// buffer to send to ICLED
//	uint16_t bit_buffer[BITS_PER_ICLED] = { 0 };			// working buffer to convert RGB-Color to ICLED Bit Frame
//
//	/* convert one RGB-color to bit pattern and place it in the bit buffer that holds the total */
//	for(uint_fast16_t k = 0; k < no_of_leds; ++k)
//	{
//		icled_convert_RGB_to_bit_code(single_wire_LED_buffer[k], bit_buffer);
//		icled_convert_bool_to_pwm_frame(bit_buffer, BITS_PER_ICLED);
//		memcpy(&pwm_dma_buffer[RESET_OFFSET + k * BITS_PER_ICLED], bit_buffer, BITS_PER_ICLED * 2);
//	}
//	icled_send_bit_stream((uint32_t*) pwm_dma_buffer, PWM_FRAME_SIZE);
//	DELAY(10);
//	return 1;
//}
