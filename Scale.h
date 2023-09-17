#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"
#include <Arduino.h>
 class Scale{
  private:
    int sckPin;
    int doutPin;
    long zeroFactor;
    float weightinKg;
    HX711 scale;
  public:
    Scale(){}
    Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg);
    void begin();
    void powerDown();
    void powerUp();
    float getWeightkg();
    
};




#endif
