#include "MyWebServer.h"
#include <WiFi.h>

MyWebServer::MyWebServer() {
    // Initialize server settings here if needed
    weightInKg = 0.0;
}

void MyWebServer::begin() {
    server.on("/", HTTP_GET, [this]() {
        String html = "<html><body>";
        html += "<h1>Weight: " + String(weightInKg, 2) + " kg</h1>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.begin();
    Serial.println("Server started");
}

void MyWebServer::displayWeight(float weightInKg) {
    this->weightInKg = weightInKg;
}

void MyWebServer::handleClient() {
    server.handleClient();
}

