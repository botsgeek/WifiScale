// AWSManager.h
#ifndef AWSMANAGER_H
#define AWSMANAGER_H


#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFiManager.h"
#include "DB_Handler.h" 

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
enum class AWSErrors{
    OK,
    CONNECTION_FAILED,
    DISCONNECTION_FAILED,
    PUBLISH_FAILED,
    INVALID_CREDENTIALS,
    INVALID_STATE,
    INVALID_ENDPOINT,
};


class AWSManager {
public:
    AWSManager(WiFiManager* wifiManager);
    AWSErrors init();
    AWSErrors connect();
    AWSErrors publish(std::string& topic, std::string& payload);
private:
    WiFiManager* wifiManager;
    WiFiClientSecure net;
    PubSubClient client;
    bool connected=false;
    
    // Add any other necessary private members or methods here
    static void messageHandler(char* topic, byte* payload, unsigned int length);
    
};

#endif

