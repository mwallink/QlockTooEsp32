#include "swirl.h"
#include "display.h"

using namespace qlocktoo;
void Swirl::setup() {
    // NOOP
}

void Swirl::loop() {
    uint16_t x_end = width - x_start;
    uint16_t y_end = height - y_start;

    uint32_t color = Display::ColorHSV(hue += 1000, saturation, brightness);
    Display::drawLine(x_start, y_start, x_end, y_end, color);
    if (x_start == width) {
        if (y_start == height) {
            x_start = 0;
            y_start = 0;
        } else {
            y_start++;
        }
    } else {
        y_start = 0;
        x_start++;
    }

    acc += (1 * dir);
    speed += acc;
    if (speed >= 100) {
        speed = 100;
        dir = -1;
    } else if (speed <= 40) {
        speed = 40;
        dir = 1;
    }
    
    Display::show();
    delay(speed);
}