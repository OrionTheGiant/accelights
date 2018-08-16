/*
 * NeoPixels.h
 *
 *  Created on: Aug 15, 2018
 *      Author: hunter
 */

#ifndef NEOPIXELS_H_
#define NEOPIXELS_H_

#include "ch.h"

typedef struct {
 uint8_t r;
 uint8_t g;
 uint8_t b;
} rgb;

typedef struct {
  float h;
  float s;
  float v;
} hsv;

rgb hsv2rgb(hsv in);




#endif /* NEOPIXELS_H_ */
