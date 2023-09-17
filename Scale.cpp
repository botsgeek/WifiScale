#include <Arduino.h>
#include "Scale.h"
#include "HX711.h"

Scale::Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg) {
    this->sckPin = sckPin;
    this->doutPin = doutPin;
    this->zeroFactor = zeroFactor;
    this->weightinKg = weightinKg;
}

void Scale::begin() {
    scale.begin(doutPin, sckPin);
    scale.set_scale(-19.84975);
    zeroFactor = scale.read_average();
    scale.set_offset(zeroFactor);
}


   void Scale::powerDown(){

     scale.power_down();
}
   void Scale::powerUp(){

     scale.power_up();
} 


float Scale::getWeightkg() {
    float weightInG = scale.get_units(10);
    weightinKg = weightInG / 1000.0;
    Serial.print("Weight of gas = ");
    Serial.print(weightinKg, 2);
    Serial.println(" kg");
    return weightinKg;
}


