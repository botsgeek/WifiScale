#include <Arduino.h>
#include "Scale.h"

Scale::Scale(int sckPin, int doutPin, long zeroFactor) {
    this->sckPin = sckPin;
    this->doutPin = doutPin;
    this->zeroFactor = zeroFactor;
    this->weightinKg = weightinKg;
    
}

void Scale::begin() {
    scale.begin(doutPin, sckPin);
    scale.set_scale(-20.05);
    //scale.set_scale(-20.25); // You can adjust the scale factor as needed
    zeroFactor = scale.read_average();
    scale.set_offset(zeroFactor);
}

void Scale::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        float weight = getWeightkg();
        }
     
    }

float Scale::getWeightkg() {
    float weightInG = scale.get_units(10);
    weightinKg = weightInG / 1000.0;
    Serial.print("Weight of gas = ");
    Serial.print(weightinKg, 2);
    Serial.println(" kg");
    return weightinKg;
}



