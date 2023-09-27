#ifndef BUZZERMANAGER_H
#define BUZZERMANAGER_H

class BuzzerManager {
public:
    BuzzerManager(int buzzerPin); // Constructor that takes the buzzer pin as an argument
    void begin(); // Initialize the buzzer
    void start(); // Start the buzzer
    void stop(); // Stop the buzzer
    void update(); // Check and handle the buzzer state
    
private:
    int buzzerPin;
    bool active;
    unsigned long startTime;
};

#endif

