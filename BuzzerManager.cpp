#include "BuzzerManager.h"
#include <Arduino.h>

BuzzerManager::BuzzerManager() {
    buzzerActive = false;
    buzzerStartTime = 0;
    buzzerInterval = 60000; // 1 minute interval
}

void BuzzerManager::begin(int buzzerPin) {
    this->buzzerPin = buzzerPin;
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

void BuzzerManager::checkAndTriggerBuzzer(float weightInKg, float lowLevel) {
    if (weightInKg <= lowLevel) {
        if (!buzzerActive) {
            buzzerStartTime = millis();
            buzzerActive = true;
            digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
        }
    } else {
        stopBuzzer();
    }
}

void BuzzerManager::stopBuzzer() {
    if (buzzerActive && millis() - buzzerStartTime >= buzzerInterval) {
        buzzerActive = false;
        digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    }
}

void BuzzerManager::update() {
    stopBuzzer(); // Check and stop the buzzer periodically
}

