// Buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(int buzzerPin);
    void startAlarm();
    void stopAlarm();

private:
    int buzzerPin;
};

#endif

