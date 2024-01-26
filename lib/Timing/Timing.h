// // timing.h
#include <String>
#include <Arduino.h>
#ifndef TIMING_H
#define TIMING_H

#include <WiFi.h>
#include "time.h"
#include <String>
#include "esp_sntp.h"



class Timing {
public:
    Timing();
    //void printLocalTime(char* timestamp);
    
    // bool getLocalTime(struct tm* timeinfo);
    std::string sendLocalTime();
    void timeavailable(struct timeval *t);
    //bool isTimeToUpdate(unsigned long updateInterval, unsigned long& previousMillis);
};






 #endif

