// WiFiManager.h

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager();
    void connect(const char* ssid, const char* password);
    bool isConnected();
};

#endif

