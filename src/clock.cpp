#include "clock.h"

#include <vector>
// #include <TimeLib.h>
// #include "WifiConfig.h"
#include <WiFi.h>

#include "RemoteDebugger.h"
#include "color.h"
#include "time.h"

#define NTP_TIMEOUT 1500

/*
   HET IS X UUR
   HET IS VIJF OVER X
   HET IS TIEN OVER X
   HET IS KWART OVER X
   HET IS TIEN VOOR HALF (X+1)
   HET IS VIJF VOOR HALF (X+1)
   HET IS HALF (X+1)
   HET IS VIJF OVER HALF (X+1)
   HET IS TIEN OVER HALF (X+1)
   HET IS KWART VOOR (X+1)
   HET IS TIEN VOOR (X+1)
   HET IS VIJF VOOR (X+1)
   HET IS (X+1) UUR
   ...
*/

#define HETIS 0
#define VIJF 13
#define TIEN 14
#define KWART 15
#define VOOR1 16
#define OVER1 17
#define HALF 18
#define UUR 19
#define VOOR2 20
#define OVER2 21

#define PIEK 0
#define BOOM 1
#define STAM 2

namespace qlocktoo {
void Clock::setup() {
    debugI("Clock setup");

    ClockConfig config;
    config.colorItIs = RGBW(0, 0, 255);
    config.colorWords = RGBW(0, 200, 0);
    config.colorHour = RGBW(255, 0, 0);
    applyConfig(config);

    // Display::begin();
    // Display::clear();
    // TODO: verplaatsen naar main
    // const long  gmtOffset_sec = 3600;
    // const int   daylightOffset_sec = 3600;
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void Clock::applyConfig(ClockConfig &config) {
    this->config = config;

    Display::clear();

    // setColor(ledsbyword[HETIS], config.colorItIs);

    uint8_t brigthness = 255;
    RgbwColor color1 = config.colorItIs.getTestColor();
    RgbwColor color2 = config.colorWords.getTestColor();
    RgbwColor color3 = config.colorHour.getTestColor();



    Display::drawPixelRaw(0, color1);
    Display::drawPixelRaw(1, color2);
    Display::drawPixelRaw(2, color3);
    Display::show();

}

void Clock::handleConfigQueue() {
    ClockConfig config;
    if (xQueueReceive(xClockConfigQueue, &config, 0) == pdFALSE) {
        return;
    }

    Serial.println("New clockconfig received from queue");
    applyConfig(config);
}

void Clock::loop() {
    if (!getLocalTime(&this->currentTime)) {
        debugD("Failed to obtain time");
        return;
    }

    handleConfigQueue();

    uint8_t hour = currentTime.tm_hour;
    uint8_t minute = currentTime.tm_min;

    // calculate target brightnesses:
    int current_hourword = hour;
    if (current_hourword > 12) current_hourword = current_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (current_hourword == 0) current_hourword = 12;                     // 0 is also called 12

    int next_hourword = hour + 1;
    if (next_hourword > 12) next_hourword = next_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (next_hourword == 0) next_hourword = 12;                  // 0 is also called 12


    // Display::show();
    delay(100);
}

uint8_t Clock::timeBrightness() {
    uint8_t hour = currentTime.tm_hour;
    uint8_t minute = currentTime.tm_min;

    if (hour > dayHour && hour < nightHour) {
        return MAX_BRIGHTNESS;
    } else if (hour < dayHour || hour > nightHour) {
        return MIN_BRIGHTNESS;
    } else if (hour == dayHour) {
        return constrain(
            map(minute, 0, 29, MIN_BRIGHTNESS, MAX_BRIGHTNESS),
            MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    } else if (hour == nightHour) {
        return constrain(
            map(minute, 0, 29, MAX_BRIGHTNESS, MIN_BRIGHTNESS),
            MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
    return 0;
}

void Clock::setColor(const std::vector<int> ddd, RGBW rgbw) {
    uint8_t brigthness = 255;
    uint32_t color = rgbw.getColor();

    for (uint8_t led = 0; led <= 110; led++) {
        //Display::drawPixelRaw(led, color, brigthness);
    }
}

}  // namespace qlocktoo