#pragma once

#include "RemoteDebugger.h"
#include "app.h"
#include "display.h"
#include "control.h"

extern QueueHandle_t xClockConfigQueue;

namespace qlocktoo {
typedef struct {
    NeoGrbwFeature::ColorObject colorItIs;
    NeoGrbwFeature::ColorObject colorWords;
    NeoGrbwFeature::ColorObject colorHour;
    NeoGrbwFeature::ColorObject colorMinutes;
} ClockConfig;

class Clock : public App {
   private:
    const static uint8_t MIN_BRIGHTNESS = 15;
    const static uint8_t MAX_BRIGHTNESS = 140;
    const static uint8_t dayHour = 8;     // Start increasing brightness
    const static uint8_t nightHour = 17;  // Start decreasing brightness

    ClockConfig config;
    RemoteDebug &Debug;
    struct tm currentTime;
    uint8_t timeBrightness();
    void setColor(const std::vector<int> leds, NeoGrbwFeature::ColorObject color);
    void handleConfigQueue();
    const std::vector<std::vector<int>> ledsbyword = {
        {10, 9, 8, 6, 5},        // HET IS
        {45, 46, 47},               // een
        {62, 63, 64, 65},           // twee
        {55, 56, 57, 58},           // drie
        {73, 74, 75, 76},           // vier
        {69, 70, 71, 72},           // vijf
        {66, 67, 68},               // zes
        {83, 84, 85, 86, 87},     // zeven
        {95, 96, 97, 98},          // acht
        {77, 78, 79, 80, 81},     // negen
        {91, 92, 93, 94},          // tien
        {88, 89, 90},              // elf
        {104, 105, 106, 107, 108, 109},  // twaalf
        {0, 1, 2, 3},            // VIJF
        {18, 19, 20, 21},            // TIEN
        {22, 23, 24, 25, 26},        // KWART
         {11, 12, 13, 14},           // VOOR1
        {29, 30, 31, 32},             // OVER1
        {40, 41, 42, 43},           // HALF
        {99, 100, 101},              // UUR
        {51, 52, 53, 54},          // VOOR2
        {33, 34, 35, 36},            // OVER2
        {112},                      // MIN1
        {112, 111},                 // MIN2
        {112, 111, 110},            // MIN3
        {112, 111, 110, 113}        // MIN4
    };

   public:
    Clock(RemoteDebug &debug) : App(Mode::CLOCK), Debug(debug) {};
    void setup();
    void loop();
    void applyConfig(ClockConfig config);
    //~Clock();
};
}