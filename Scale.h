// Scale.h

#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"
#include <Arduino.h>
#include "WiFiManager.h" // Include WiFiManager header
#include "MyWebServer.h" 
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
    unsigned long interval;
    unsigned long lastGaugeUpdateMillis; // Store the last gauge update time
    WiFiManager &wifiManager; // Reference to the WiFiManager object
    MyWebServer &server; // Reference to the MyWebServer object
    bool isPowerDown; // Declare isPowerDown as a class member

public:
    Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg, WiFiManager &wifiManagerRef, MyWebServer &serverRef);
    void begin();
    void update();
    void powerDown();
    void powerUp();
    float getWeightkg();
};

#endif

