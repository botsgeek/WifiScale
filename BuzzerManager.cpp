#include "BuzzerManager.h"
#include <Arduino.h>

BuzzerManager::BuzzerManager(int buzzerPin) : buzzerPin(buzzerPin), active(false), startTime(0) {
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

void BuzzerManager::begin() {
    // Initialize any buzzer-related settings here, if needed
}

void BuzzerManager::start() {
    active = true;
    startTime = millis();
    digitalWrite(buzzerPin, HIGH);
}

void BuzzerManager::stop() {
    active = false;
    digitalWrite(buzzerPin, LOW);
}

void BuzzerManager::update() {
    if (active && millis() - startTime >= 1000) { // Turn off the buzzer after 1 second (adjust as needed)
        stop();
    }
}

