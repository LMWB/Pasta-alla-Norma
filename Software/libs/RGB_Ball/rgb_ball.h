#ifndef RGB_BALL_RGB_BALL_H_
#define RGB_BALL_RGB_BALL_H_

#include "platformGlue.h"

void rgb_ball_init(void);
uint8_t rgb_ball_read_jumpers(void);
void rgb_ball_blink(uint8_t how_often);

void rgb_ball_main(void);

void rgb_ball_daylight(void);
void rgb_ball_sun_storm(void);
void sunstorm_cycle(void);
void rgb_ball_christmas_color_fade(void);
void rgb_ball_rainbow_fade(void);
void Haunted_Pickle(void);

#endif /* RGB_BALL_RGB_BALL_H_ */
