// .ino

#include <Arduino.h>
#include "Scale.h"
#include "WiFiManager.h"
#include "MyWebServer.h"
#include "BuzzerManager.h"

float yourLowLevel = 0.0;
const int sckPin = 4;
const int doutPin = 16;
const int buzzerPin = 12; // Define your buzzer pin here
const char* yourSSID = "DAVID";
const char* password = "Olakunle001";


WiFiManager wifiManager;
MyWebServer server;
Scale myScale(sckPin, doutPin, 0, 0, wifiManager, server);
BuzzerManager buzzerManager;

void setup() {
    Serial.begin(115200);
    Serial.println("Weight of Gas Loading");
    Serial.println("Initializing the scale");
    myScale.begin();
    wifiManager.connect(yourSSID, password);
    server.begin();
    buzzerManager.begin(buzzerPin); // Initialize the buzzer
}

void loop() {
    myScale.update();
    server.handleClient();
    buzzerManager.checkAndTriggerBuzzer(myScale.getWeightkg(), yourLowLevel); // Check and trigger the buzzer
    // Add any other code you want to run continuously here
}

