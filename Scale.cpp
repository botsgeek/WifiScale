#include <Arduino.h>
#include "Scale.h"
#include "HX711.h"
#include "BuzzerManager.h"

Scale::Scale(int sckPin, int doutPin, long zeroFactor, float weightinKg, WiFiManager &wifiManagerRef, MyWebServer &serverRef, BuzzerManager &buzzerManagerRef)
    : wifiManager(wifiManagerRef), server(serverRef), isPowerDown(false), buzzerManager(buzzerManagerRef), buzzerActive(false), buzzerStartTime(0), buzzerInterval(60000) {
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
    scale.set_scale(-20.25); // You can adjust the scale factor as needed
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
            server.displayWeight(weight);
            powerDown(); // You can uncomment this if needed
        }
        isPowerDown = !isPowerDown;
    }

    // Check if it's time to update the gauge (every 10 seconds)
    if (currentMillis - lastGaugeUpdateMillis >= 10000) {
        lastGaugeUpdateMillis = currentMillis;

        // Update the gauge here
        if (wifiManager.isConnected()) {
            server.displayWeight(weightinKg);
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

void Scale::checkAndTriggerBuzzer(float weightInKg, float lowLevel) {
    if (weightInKg <= lowLevel) {
        if (!buzzerActive) {
            buzzerStartTime = millis();
            buzzerActive = true;
            buzzerManager.start(); // Start the buzzer
        }
    } else {
        stopBuzzer();
    }
}

void Scale::stopBuzzer() {
    if (buzzerActive) {
        unsigned long currentTime = millis();
        unsigned long buzzerDuration = currentTime - buzzerStartTime;

        if (buzzerDuration >= 10000) { // Check if the buzzer has been active for 10 seconds
            buzzerManager.stop(); // Stop the buzzer
            buzzerActive = false;
        }
    } else {
        unsigned long currentTime = millis();
        unsigned long timeSinceLastStop = currentTime - buzzerStartTime;

        if (timeSinceLastStop >= 1800000UL) { // Check if 30 minutes have passed since the last buzzer stop
            buzzerStartTime = currentTime;
        }
    }
}

