#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"
#include <Arduino.h>
#include "WiFiManager.h"
#include "MyWebServer.h"
#include "BuzzerManager.h"

// Forward declaration of MyWebServer class
class MyWebServer;

class Scale {
private:
    int sckPin;
    int doutPin;
    long zeroFactor;
    float weightinKg;
    HX711 scale;
    unsigned long previousMillis;
     unsigned long interval = 1800000;
    unsigned long lastGaugeUpdateMillis;
    WiFiManager &wifiManager;
    MyWebServer &server;
    bool isPowerDown;
    BuzzerManager &buzzerManager; // Reference to BuzzerManager
    bool buzzerActive; // Declare buzzerActive as a class member
    unsigned long buzzerStartTime; // Declare buzzerStartTime as a class member
    unsigned long buzzerInterval; // Declare buzzerInterval as a class member


public:
    Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg, WiFiManager &wifiManagerRef, MyWebServer &serverRef, BuzzerManager &buzzerManagerRef);
    void begin();
    void update();
    void powerDown();
    void powerUp();
    float getWeightkg();
    void checkAndTriggerBuzzer(float weightInKg, float lowLevel);
    void stopBuzzer();
};

#endif

