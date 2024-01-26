// Buzzer.cpp
#include "Buzzer.h"

Buzzer::Buzzer(int buzzerPin) {
    this->buzzerPin = buzzerPin;
    pinMode(buzzerPin, OUTPUT);
}

void Buzzer::startAlarm() {
    digitalWrite(buzzerPin, HIGH);
}

void Buzzer::stopAlarm() {
    digitalWrite(buzzerPin, LOW);
}

