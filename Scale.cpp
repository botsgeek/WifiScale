// Scale.cpp

#include <Arduino.h>
#include "Scale.h"
#include "HX711.h"

Scale::Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg, WiFiManager &wifiManagerRef, MyWebServer &serverRef)
    : wifiManager(wifiManagerRef), server(serverRef), isPowerDown(false) { // Initialize isPowerDown in the constructor
    this->sckPin = sckPin;
    this->doutPin = doutPin;
    this->zeroFactor = zeroFactor;
    this->weightinKg = weightinKg;
    this->previousMillis = 0;
    this->interval = 10000; // 10 seconds interval
    this->lastGaugeUpdateMillis = 0;
}

void Scale::begin() {
    scale.begin(doutPin, sckPin);
    scale.set_scale(-19.84975);
    zeroFactor = scale.read_average();
    scale.set_offset(zeroFactor);
}

void Scale::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (isPowerDown) {
            powerUp();
        } else {
            float weight = getWeightkg();
            powerDown();
        }
        isPowerDown = !isPowerDown;
    }

    // Check if it's time to update the gauge (every 10 seconds)
    if (currentMillis - lastGaugeUpdateMillis >= 10000) {
        lastGaugeUpdateMillis = currentMillis; // Update the last gauge update time

        // Update the gauge here
        if (wifiManager.isConnected()) {
            server.displayWeight(weightinKg); // Use the weightinKg stored in the Scale instance
            server.handleClient();
        }
    }
}

void Scale::powerDown() {
    scale.power_down();
}

void Scale::powerUp() {
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

