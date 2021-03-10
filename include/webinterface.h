#pragma once

#include <ESPAsyncWebServer.h>
#include "RemoteDebugger.h"

using namespace std;

extern QueueHandle_t xChangeAppQueue;
extern QueueHandle_t xClockConfigQueue;

namespace qlocktoo {
class Webinterface {
   private:
    const int timeout = 2000;  // timout for connections in milliseconds
    const char *KEY_HUE = "h";
    const char *KEY_SATURATION = "s";
    const char *KEY_VALUE = "v";

    RemoteDebug &Debug;
    AsyncWebServer server;
    
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

   public:
    Webinterface(int port, RemoteDebug &debug_);
    void begin();
};
}