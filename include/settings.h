#pragma once

#include "Arduino.h"
#include <NeoPixelBrightnessBus.h>

// Tells how the LED's are arranged over the board.
enum MatrixMode { RIGHT_TO_LEFT, LEFT_TO_RIGHT, FREESTYLE_RICK };

// Pins on the ESP
const uint8_t LEDSTRIP_PIN = 25;
const uint8_t BUTTON1_PIN = -1;
const uint8_t BUTTON2_PIN = -1;
const uint8_t BUTTON3_PIN = -1;
const uint8_t BUTTON4_PIN = -1;
const uint8_t TOUCH_PIN = -1;
const uint8_t LIGHT_SENSOR_PIN = -1;

// Number of leds on the strip. (110 for the matrix only, 114 including the corner leds.)
const int LED_COUNT = 114;

// The order in which minute LED should turn on first.
const int CORNER_ORDER [4] = {110, 111, 112, 113};

// How are the LED's arranged over the board.
const MatrixMode MATRIX_MODE = RIGHT_TO_LEFT;

// Default 'it is' color.
NeoGrbwFeature::ColorObject DEFAULT_COLOR_ITIS = RgbwColor(0, 0, 0, 40);
// Default 'words' color.
NeoGrbwFeature::ColorObject DEFAULT_COLOR_WORDS = RgbwColor(0, 0, 0, 40);
// Default 'hour' color.
NeoGrbwFeature::ColorObject DEFAULT_COLOR_HOUR = RgbwColor(0, 0, 0, 40);
// Default 'minutes' color.
NeoGrbwFeature::ColorObject DEFAULT_COLOR_MINUTES = RgbwColor(0, 0, 0, 40);
