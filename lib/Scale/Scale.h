#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"
#include <Arduino.h>


class Scale {
private:
    int sckPin;
    int doutPin;
    long zeroFactor;
    float weightinKg;
    HX711 scale;
    unsigned long previousMillis;
    unsigned long interval;


public:
    //float weightinKg;
    Scale(int sckPin, int doutPin, long zeroFactor);
    void begin();
    void update();
    float getWeightkg();
};

#endif

