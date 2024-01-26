// WiFiManager.h

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <Arduino.h>
#define WIFI_COUNTER_TIMEOUT 60
enum class WiFiErrors{
    OK,
    CONNECTION_FAILED,
    DISCONNECTION_FAILED,
    INVALID_PASSWORD,
    INVALID_SSID
};
class WiFiManager{
public:
    WiFiManager(std::string SSID, std::string password);
    WiFiErrors init();
    WiFiErrors connect();
    WiFiErrors disconnect();
    bool isConnected();
private:
    std::string password;
    std::string SSID;
};



#endif
