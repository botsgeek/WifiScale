// MyWebServer.cpp

#include "MyWebServer.h"
#include <WiFi.h>

MyWebServer::MyWebServer() {
    // Initialize server settings here if needed
    weightInKg = 0.0;
    lowLevel = 0.0; // Initialize the low-level threshold to 0
}

void MyWebServer::begin() {
    server.on("/", HTTP_GET, [this]() {
        String html = "<html><body>";
        html += "<h1>Weight: " + String(weightInKg, 2) + " kg</h1>";
        html += "<p>Low Level Threshold: " + String(lowLevel, 2) + " kg</p>";
        html += "<a href='/pages/about.html'>Settings</a>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/pages/about.html", HTTP_GET, [this]() {
        String html = "<html><body>";
        html += "<h2>Settings</h2>";
        html += "<form action='/setThreshold' method='POST'>";
        html += "<label for='low_level'>Low Level (kg):</label>";
        html += "<input type='number' id='low_level' name='low_level' step='0.01' required>";
        html += "<br>";
        html += "<input type='submit' value='Set Threshold'>";
        html += "</form>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/setThreshold", HTTP_POST, [this]() {
        if (server.hasArg("low_level")) {
            float newLowLevel = server.arg("low_level").toFloat();
            lowLevel = newLowLevel;// Call the setLowLevel function to update the threshold
            Serial.print("Low Level set to: ");
            Serial.println(lowLevel);
        }
        server.send(200, "text/html", "Threshold Set");
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

void MyWebServer::setLowLevel(float lowLevel) {
    this->lowLevel = lowLevel;
}

float MyWebServer::getLowLevel() {
    return lowLevel;
}

