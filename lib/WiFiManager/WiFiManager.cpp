#include "WiFiManager.h"


WiFiManager::WiFiManager(std::string SSID, std::string password):SSID(SSID),password(password)
{
    
}
WiFiErrors WiFiManager::init()
{

    if(password.empty()|| password.length() < 8){
        return WiFiErrors::INVALID_PASSWORD;
    }
    if(SSID.empty()|| SSID.length() < 5){
        return WiFiErrors::INVALID_SSID;
    }
    return WiFiErrors::OK;

}
WiFiErrors WiFiManager::connect(){
    if(WiFi.isConnected()){
        return WiFiErrors::OK;
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID.c_str(), password.c_str());

    Serial.println("Connecting to WiFi...");
    uint8_t counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter < WIFI_COUNTER_TIMEOUT ) {
    delay(1000);
    counter++;
    Serial.println("Connecting...");
    }
    if(counter >= WIFI_COUNTER_TIMEOUT){
        return WiFiErrors::CONNECTION_FAILED;
    }else{
        return WiFiErrors::OK;
    }

}

WiFiErrors WiFiManager::disconnect(){
    if(!WiFi.isConnected()){
        return WiFiErrors::OK;
    }
    return WiFi.disconnect()?WiFiErrors::OK:WiFiErrors::DISCONNECTION_FAILED;
}
bool WiFiManager::isConnected(){
    return WiFi.isConnected();
}