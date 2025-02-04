#include "webinterface.h"
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "RemoteDebugger.h"
#include "clock.h"
#include "control.h"

namespace qlocktoo {
Webinterface::Webinterface(int port, RemoteDebug &debug_) : Debug(debug_), server(AsyncWebServer(port)) {
    // Webinterface::Webinterface(int port) {
    //server = AsyncWebServer(port);
    // auto ws = AsyncWebSocket("/ws");
    // auto event = AsyncEventSource("/events");

    // RemoteDebug Debug;
    debugE("HALLO vanuit webinterface");
    // debugI
}

void Webinterface::begin() 
{
    //Preferences preferences;

    server.serveStatic("/", SPIFFS, "/qlocktoo-portal").setDefaultFile("index.html");
    // server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
    //     debugI("GET received :)");
    //     request->send(200, "text/plain", "Hello, you rang?");
    // });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [&](AsyncWebServerRequest *request) {
        String message;
        debugI("GET with message :)");
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // server.onRequestBody(
    // [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)

    // [this](const String& var) { return processor(var); }

    server.onRequestBody(
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            if ((request->url() == "/api/clock") &&
                (request->method() == HTTP_POST)) {
                StaticJsonDocument<512> jsonDoc;

                if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
                    ClockConfig config;
                    float h, s, v;

                    auto color = jsonDoc["colorItIs"];
                    h = color["h"] | 0.0f;
                    s = color["s"] | 0.0f;
                    v = color["v"] | 0.0f;
                    config.colorItIs = HsbColor(h, s, v);
                    // Serial.printf("RGB it is: %u, %u, %u, %u\r\n", r, g, b, w);

                    color = jsonDoc["colorWords"];
                    h = color["h"] | 0.0f;
                    s = color["s"] | 0.0f;
                    v = color["v"] | 0.0f;
                    config.colorWords = HsbColor(h, s, v);
                    // Serial.printf("RGB words: %u, %u, %u, %u\r\n", r, g, b, w);
                    float ref = 3.23f;
                    debugD("Reftest: %f", ref);
                    debugD("HSV: %f, %f, %f", h, s, v);

                    color = jsonDoc["colorHour"];
                    h = color["h"] | 0.0f;
                    s = color["s"] | 0.0f;
                    v = color["v"] | 0.0f;
                    config.colorHour = HsbColor(h, s, v);
                    // Serial.printf("RGBW hour: %u, %u, %u, %u\r\n", r, g, b, w);

                    color = jsonDoc["colorMinutes"];
                    h = color["h"] | 0.0f;
                    s = color["s"] | 0.0f;
                    v = color["v"] | 0.0f;
                    config.colorMinutes = HsbColor(h, s, v);
                    // Serial.printf("RGBW hour: %u, %u, %u, %u\r\n", r, g, b, w);

                    // TODO: iets met kleurtjes
                    Mode newMode = Mode::CLOCK;
                    // xQueueSend(xChangeAppQueue, &newMode, 0);
                    xQueueSend(xClockConfigQueue, &config, 0);
                }

                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }

            if ((request->url() == "/api/swirl") &&
                (request->method() == HTTP_POST)) {
                // StaticJsonDocument<512> jsonDoc;

                // if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char*)data))
                {
                    debugI("Mode set to SWIRL");
                    // setMode(qlocktoo::SWIRL);
                    xQueueSend(xChangeAppQueue, (void *) Mode::SWIRL, 0);
                }

                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }
            if ((request->url() == "/api/test") &&
                (request->method() == HTTP_POST)) {
                // StaticJsonDocument<512> jsonDoc;
                Serial.println("REST API - test");
                Serial.print("Address of xChangeAppQueue: ");
                Serial.println((int) &xChangeAppQueue);

                // if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char*)data))
                {
                    Mode test = Mode::CLOCK;
                    bool success = (xQueueSend(xChangeAppQueue, &test, 0) == pdTRUE);
                    
                    testswitch = !testswitch;

                    Serial.printf("Send to queue: %s\r\n", success ? "true" : "false");
                }

                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }

            if ((request->url() == "/api/leds") &&
                (request->method() == HTTP_POST)) {
                StaticJsonDocument<512> jsonDoc;

                if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
                    uint8_t leds, dur1, dur2, dur3, dur4;
                    auto config = jsonDoc["config"];
                    leds = config["leds"] | 10;
                    dur1 = config["dur1"] | 10;
                    dur2 = config["dur2"] | 10;
                    dur3 = config["dur3"] | 10;
                    dur4 = config["dur4"] | 10;

                    debugI("durations: %u, %u, %u, %u", dur1, dur2, dur3, dur4);
                    Serial.printf("leds: %d, durations: %d, %d, %d, %d", leds, dur1, dur2, dur3, dur4);
                    // Display::setConfig(110, dur1, dur2, dur3, dur4);
                    // Display::clear();
                    // Display::show();
                    Display::setConfig(leds, dur1, dur2, dur3, dur4);
                    Display::show();
                }

                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }

            if ((request->url() == "/api/reboot") &&
                (request->method() == HTTP_GET)) 
            {
                ESP.restart();
            }

        });

    server.onNotFound([&](AsyncWebServerRequest *request) {
        debugI("Not found");
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
}

void Webinterface::test(const char *tekst) {
    debugE("Dit is een test vanuit de webinterface");

    debugE("Passed: %s", tekst);
}

void Webinterface::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("Hello Client %u :)", client->id());
        client->ping();
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    } else if (type == WS_EVT_ERROR) {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    } else if (type == WS_EVT_PONG) {
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len) {
            //the whole message is in a single frame and we got all of it's data
            Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

            if (info->opcode == WS_TEXT) {
                for (size_t i = 0; i < info->len; i++) {
                    msg += (char)data[i];
                }
            } else {
                char buff[3];
                for (size_t i = 0; i < info->len; i++) {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if (info->opcode == WS_TEXT)
                client->text("I got your text message");
            else
                client->binary("I got your binary message");
        } else {
            //message is comprised of multiple frames or the frame is split into multiple packets
            if (info->index == 0) {
                if (info->num == 0)
                    Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
            }

            Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

            if (info->opcode == WS_TEXT) {
                for (size_t i = 0; i < len; i++) {
                    msg += (char)data[i];
                }
            } else {
                char buff[3];
                for (size_t i = 0; i < len; i++) {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if ((info->index + len) == info->len) {
                Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
                if (info->final) {
                    Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                    if (info->message_opcode == WS_TEXT)
                        client->text("I got your text message");
                    else
                        client->binary("I got your binary message");
                }
            }
        }
    }
}
}