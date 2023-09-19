#include <Arduino.h>
#include "Scale.h"
#include "WiFiManager.h"
#include "MyWebServer.h"

const int sckPin = 4;
const int doutPin = 16;
const char* yourSSID = "DAVID";
const char* password = "Olakunle001";

WiFiManager wifiManager;
MyWebServer server;
Scale myScale(sckPin, doutPin, 0, 0, wifiManager, server); ; // Pass WiFiManager by reference

void setup() {
    Serial.begin(115200);
    Serial.println("Weight of Gas Loading");
    Serial.println("Initializing the scale");
    myScale.begin();
    wifiManager.connect(yourSSID, password);
    server.begin();
}

void loop() {
    myScale.update();
    // Add any other code you want to run continuously here
}

