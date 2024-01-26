#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include "SDCardManager.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

class MyWebServer {
public:
    MyWebServer (SDCardManager &sdCardManager);
    void begin(AsyncWebServer &server);
    void displayWeight(float weightInKg);
    void setLowLevel(float lowLevel); // Function to set the low-level threshold
    float getLowLevel(); // Function to get the low-level threshold
    void setEmptyLevel(float emptyLevel); // Function to set the empty level
    float getEmptyLevel(); // Function to get the empty level
    void setFullLevel(float fullLevel); // Function to set the full level
    float getFullLevel(); // Function to get the full level


private:
    float weightInKg;
    float lowLevel; // Store the low-level threshold
    float emptyLevel; // Store the empty level
    float fullLevel; // Store the full level
    SDCardManager &sdCardManager;
};  

#endif

