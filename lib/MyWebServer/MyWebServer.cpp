#include "MyWebServer.h"
#include <WiFi.h>
#include "Scale.h"
#include <ArduinoJson.h>

extern Scale myScale;
MyWebServer::MyWebServer(SDCardManager &sdCardManager) : sdCardManager(sdCardManager) {
 

}


void MyWebServer::begin(AsyncWebServer &server) {
    sdCardManager.initSDCard();
   
     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

   server.on("/getWeight", HTTP_GET, [](AsyncWebServerRequest *request){
        float currentWeight = myScale.getWeightkg();
        request->send(200, "text/plain", String(currentWeight));
        });

//    server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", "esp32");
//    });

     server.on("/setConfig", HTTP_POST, [](AsyncWebServerRequest *request){
        // Handle the POST request for configuration
        String json = request->arg("plain");
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json);

        float lowLevel = doc["low_level"];
        float emptyLevel = doc["empty_level"];
        float fullLevel = doc["full_level"];

        // Use these values as needed in your Arduino code
        // Example: myScale.setLowLevel(lowLevel)
        request->send(200, "text/plain", "Configuration settings updated");
    });

//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Hello! This is a simple text message sent from ESP32. Microcontrollerslab");
//    });
       

    server.serveStatic("/", SD, "/");

  server.begin();
}
void MyWebServer::displayWeight(float weightInKg) {
    this->weightInKg = weightInKg;
}


void MyWebServer::setLowLevel(float lowLevel) {
    this->lowLevel = lowLevel;
}

float MyWebServer::getLowLevel() {
    return lowLevel;
}
void MyWebServer::setEmptyLevel(float emptyLevel) {
    this->emptyLevel = emptyLevel;
}

float MyWebServer::getEmptyLevel() {
    return emptyLevel;
}

void MyWebServer::setFullLevel(float fullLevel) {
    this->fullLevel = fullLevel;
}

float MyWebServer::getFullLevel() {
    return fullLevel;
}
