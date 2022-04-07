#include "display.h"
#include <NeoPixelBrightnessBus.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif

namespace qlocktoo {
    static uint16_t getLedByCoordinate(uint16_t x, uint16_t y) {
        static uint8_t mapping_rick[11][10] = {
            {73, 80, 102, 96, 58, 51, 22, 7, 30, 0},
            {81, 66, 65, 88, 89, 15, 14, 44, 43, 37},
            {72, 103, 82, 59, 95, 21, 50, 29, 6, 38},
            {74, 79, 101, 97, 57, 52, 23, 8, 31, 1},
            {104, 67, 64, 87, 90, 16, 13, 45, 42, 36},
            {71, 109, 83, 60, 94, 20, 49, 28, 5, 39},
            {75, 78, 100, 98, 56, 53, 24, 9, 32, 2},
            {105, 68, 63, 86, 91, 17, 12, 46, 41, 35},
            {70, 108, 84, 61, 93, 19, 48, 27, 4, 40},
            {76, 77, 107, 99, 55, 54, 25, 10, 33, 3},
            {106, 69, 62, 85, 92, 18, 11, 47, 26, 34}};

        static uint8_t mapping_left[11][10] = {
            {0 ,  1,  2,  3,  4,  5,  6,  7,  8,  9},
            {10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
            {20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
            {30, 33, 32, 33, 34, 35, 36, 37, 38, 39},
            {40, 44, 42, 43, 44, 45, 46, 47, 48, 49},
            {50, 55, 52, 53, 54, 55, 56, 57, 58, 59},
            {60, 66, 62, 63, 64, 65, 66, 67, 68, 69},
            {70, 77, 72, 73, 74, 75, 76, 77, 78, 79},
            {80, 88, 82, 83, 84, 85, 86, 87, 88, 89},
            {90, 99, 92, 93, 94, 95, 96, 97, 98, 99},
            {100, 101, 102, 103, 104, 105, 106, 107, 108, 109}
        };

        static uint8_t mapping_right[11][10] = {
            {9 ,  8,  7,  6,  5,  4,  3,  2,  1,  0},
            {19, 18, 17, 16, 15, 14, 13, 12, 11, 10},
            {29, 28, 27, 26, 25, 24, 23, 22, 21, 20},
            {39, 38, 37, 36, 35, 34, 33, 32, 31, 30},
            {49, 48, 47, 46, 45, 44, 43, 42, 41, 40},
            {59, 58, 57, 56, 55, 54, 53, 52, 51, 50},
            {69, 68, 67, 66, 65, 64, 63, 62, 61, 60},
            {79, 78, 77, 76, 75, 74, 73, 72, 71, 70},
            {89, 88, 87, 86, 85, 84, 83, 82, 81, 80},
            {99, 98, 97, 96, 95, 94, 93, 92, 91, 90},
            {109, 108, 107, 106, 105, 104, 103, 102, 101, 100}
        };



        return mapping_right[x][9 - y];
    }


    NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> Display::realDisplay(114, LEDSTRIP_PIN);
    bool Display::initialized = false;

    void Display::begin() {
        if (!initialized) {
            RgbColor dark(2, 2, 0);
            realDisplay.Begin();
            // realDisplay.SetBrightness(255);
            realDisplay.ClearTo(dark);
            realDisplay.Show();
            // realDisplay.setRemapFunction(getLedByCoordinate);
            // realDisplay.Begin begin(LEDSTRIP_PIN, WIDTH * HEIGHT);
            initialized = true;
        }
    }

    void Display::drawPixel(int16_t x, int16_t y, NeoGrbwFeature::ColorObject c) {
        drawPixel(getLedByCoordinate(x, y), c);
    };

    void Display::drawPixel(int16_t index, NeoGrbwFeature::ColorObject c) {
        realDisplay.SetPixelColor(index, c);
    }

    void Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, NeoGrbwFeature::ColorObject color) {
        int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            _swap_int16_t(x0, y0);
            _swap_int16_t(x1, y1);
        }

        if (x0 > x1) {
            _swap_int16_t(x0, x1);
            _swap_int16_t(y0, y1);
        }

        int16_t dx, dy;
        dx = x1 - x0;
        dy = abs(y1 - y0);

        int16_t err = dx / 2;
        int16_t ystep;

        if (y0 < y1) {
            ystep = 1;
        } else {
            ystep = -1;
        }

        for (; x0 <= x1; x0++) {
            if (steep) {
                drawPixel(y0, x0, color);
            } else {
                drawPixel(x0, y0, color);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
    }
}