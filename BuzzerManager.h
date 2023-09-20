#ifndef BUZZERMANAGER_H
#define BUZZERMANAGER_H

class BuzzerManager {
public:
    BuzzerManager();
    void begin(int buzzerPin);
    void checkAndTriggerBuzzer(float weightInKg, float lowLevel);
    void stopBuzzer();
    void update();
    
private:
    int buzzerPin;
    bool buzzerActive;
    unsigned long buzzerStartTime;
    unsigned long buzzerInterval;
};

#endif

