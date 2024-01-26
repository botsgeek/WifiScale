// AWSManager.cpp
#include "AWSManager.h"
#include "secrets.h"
#define AWS_TIMOUT_COUNTER 3
AWSManager::AWSManager(WiFiManager* wifiManager):wifiManager(wifiManager)
{

}

AWSErrors AWSManager::init(){
    Serial.println("in aws init");
    if(!strlen(AWS_CERT_CA) || !strlen(AWS_CERT_CRT) || !strlen(AWS_CERT_PRIVATE)){
        Serial.println("one of the credential is bad");
        return AWSErrors::INVALID_CREDENTIALS;
    }
    if(!strlen(AWS_IOT_ENDPOINT))
    {
        Serial.println("bad endpoint");
        return AWSErrors::INVALID_ENDPOINT;
    }
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);
    this->client = client.setClient(net);
    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    this->client.setServer(AWS_IOT_ENDPOINT, 8883);

    // Create a message handler
    this->client.setCallback(messageHandler); 
    Serial.println("retruning OK");
    
    return AWSErrors::OK;
}
AWSErrors AWSManager::connect()
{
    if(connected){
        Serial.println("client already connected");
        return AWSErrors::OK;
    }
    if(!wifiManager->isConnected())
    {
        Serial.println("wifi not connected");
        return AWSErrors::INVALID_STATE;
    }

    uint8_t counter = 0;
   while (!client.connect(THINGNAME) && counter < AWS_TIMOUT_COUNTER ) {
        Serial.print(".");
        counter++;
        delay(1000);
    }
    if(counter >= AWS_TIMOUT_COUNTER){
        Serial.println("aws client timeout");
        return AWSErrors::CONNECTION_FAILED;
    }else{
        Serial.println("aws client connection ok");
        connected= true;
        return AWSErrors::OK;
    }  

}
AWSErrors AWSManager::publish(std::string& topic, std::string& payload){
    if(!connected){
        return AWSErrors::INVALID_STATE;
    }
    uint32_t payload_length = payload.length();
    bool result = client.beginPublish(topic.c_str(),payload_length,false);
    if(!result){
        Serial.println("unable to begin publish");
        return AWSErrors::PUBLISH_FAILED;
    }
    Serial.printf("total payload length is: %d\n",payload_length);
    uint32_t counter = 0;
    uint8_t step_size = 10;
    uint32_t max_index = payload_length;
    while(counter < max_index){
        uint32_t end_index = counter+step_size;
        if(end_index > max_index)
        {
            end_index = max_index;
        }
        size_t sent = client.print(payload.substr(counter,end_index).c_str());
        Serial.printf("sent %d bytes\n",sent);
        counter = end_index;
        vTaskDelay(1);
    }
    result = client.endPublish();
    if(!result){
        Serial.println("unable to end publish");
        return AWSErrors::PUBLISH_FAILED;
    }
    Serial.println("published successful");
    return AWSErrors::OK;
}
void AWSManager::messageHandler(char* topic, byte* payload, unsigned int length) {
    // Your message handling code here
    Serial.print("incoming: ");
    // Serial.println(topic);

    // StaticJsonDocument<200> doc;
    // deserializeJson(doc, payload);
    // const char* message = doc["message"];
    // Serial.println(message);
}
