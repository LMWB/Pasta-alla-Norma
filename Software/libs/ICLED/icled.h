#ifndef ICLED_ICLED_H_
#define ICLED_ICLED_H_

#include <stdint.h>
#include "icled_config.h"

uint8_t icled_apply_brightness(uint16_t position, uint8_t brightness);
uint8_t icled_set_color(pixel_t* pixel, uint16_t position);
uint8_t icled_write_pixel_buffer_to_pwm(void);
uint8_t icled_write_n_pixel_buffer_to_pwm(uint8_t no_of_leds);

#endif /* ICLED_ICLED_H_ */
