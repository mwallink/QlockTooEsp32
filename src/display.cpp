#include "display.h"
#include <NeoPixelBrightnessBus.h>

namespace qlocktoo {
    static uint16_t getLedByCoordinate(uint16_t x, uint16_t y) {
        static uint8_t mapping[11][10] = {
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

        return mapping[x][9 - y];
    }


    NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> Display::realDisplay(110, LEDSTRIP_PIN);
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

    void Display::drawPixel(int16_t x, int16_t y, uint32_t c) {
        uint8_t index = getLedByCoordinate(x, y);
        realDisplay.SetPixelColor(index, c);
    };

    void Display::drawPixelRaw(int16_t index, RgbwColor c) {
        realDisplay.SetPixelColor(index, c);
    }

    void Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
        // TODO
    }
}