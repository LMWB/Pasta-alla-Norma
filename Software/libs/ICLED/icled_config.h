#ifndef ICLED_ICLED_CONFIG_H_
#define ICLED_ICLED_CONFIG_H_

#include <stdint.h>

// Define the size of the ICLED array that is being used
#define MAX_NO_OF_LEDS	8

typedef struct {
	uint8_t F_C;	// control byte for brightness 0..100%, ICLED_dual uses a slightly different approach
	uint8_t B;		// byte for color blue
	uint8_t G;		// byte for color green
	uint8_t R;		// byte for color red
} pixel_t;  		// total size per LED is 4 Byte

#endif /* ICLED_ICLED_CONFIG_H_ */
