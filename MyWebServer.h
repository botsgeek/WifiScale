#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

class MyWebServer {
public:
    MyWebServer();
    void begin();
    void handleClient();
    void displayWeight(float weightInKg);

private:
    WebServer server;
    float weightInKg;
};

#endif

