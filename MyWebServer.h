// MyWebServer.h

#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

class MyWebServer {
public:
    MyWebServer();
    void begin();
    void handleClient();
    void displayWeight(float weightInKg);
    void setLowLevel(float lowLevel); // Function to set the low-level threshold
    float getLowLevel(); // Function to get the low-level threshold

private:
    WebServer server;
    float weightInKg;
    float lowLevel; // Store the low-level threshold
};

#endif

