#ifndef ICLED_ICLED_CONFIG_H_
#define ICLED_ICLED_CONFIG_H_

#include <stdint.h>

// Define the size of the ICLED array that is being used
#define MAX_NO_OF_LEDS	8

typedef struct {
	uint8_t R;		// byte for color red
	uint8_t G;		// byte for color green
	uint8_t B;		// byte for color blue
} pixel_t;  		// total size per LED is 4 Byte

#endif /* ICLED_ICLED_CONFIG_H_ */
