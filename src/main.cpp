
#include <Arduino.h>
#include "WiFiManager.h"
#include "Timing.h"
#include "DB_Handler.h"
#include "SDCardManager.h"
#include "MyWebServer.h"
#include "Scale.h"
#include "AWSManager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 0;
constexpr const char *DATABASE_PATH = "/sd/gas_db.db";
constexpr const char *TABLE_NAME = "gas_reading";

int serialNumber = 1;
const char *your_timestamp = "2023-10-25 16:19:00";

unsigned long updateInterval = 120000; // Define the update interval in milliseconds (2 minutes)
unsigned long previousMillis = 0;
std::string publish_topic = "esp32/pub";
const int sckPin = 4;
const int doutPin = 16;
std::string SSID = "RILLO";
std::string password = "rillo@1940";
static bool deleteBool = false;
// AsyncWebServer server(80);
WiFiManager wifiManager(SSID, password);
Timing timing;
DB db_Handler(DATABASE_PATH, TABLE_NAME);
SDCardManager sdCardManager;
// MyWebServer myServer(sdCardManager);
Scale myScale(sckPin, doutPin, 0);

TaskHandle_t scaleTaskHandle;
TaskHandle_t publishTaskHandle;
// AWSManager awsManager(wifiManager);
AWSManager awsManager(&wifiManager);

QueueHandle_t weightQueue;
// QueueHandle_t timeQueue;
void scaleTask(void *parameter)
{
  float weight;
  // std::string local_time;
  std::string timestamp;
  while (true)
  {
    Serial.printf("inside %s\n", __func__);
    weight = myScale.getWeightkg();
    // timestamp = timing.sendLocalTime();
    // weight = 100;

    xQueueSend(weightQueue, &weight, pdMS_TO_TICKS(portMAX_DELAY));
    // xQueueSend(timeQueue, &timestamp, pdMS_TO_TICKS(portMAX_DELAY));
    Serial.printf("weight sent to queue: %f\n", weight);
    // Serial.printf("time sent to queue: %s\n", timestamp);

    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 5 seconds
                                          // myScale.update();
    // vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
  }
}
void connectivityTask(void *parameter)
{
  // AWSErrors publishMessage(std::string topic, std::string payload);
  WiFiErrors err = wifiManager.init();
  while (err != WiFiErrors::OK)
  {
  }
  err = wifiManager.connect();
  while (err != WiFiErrors::OK)
  {
  }
  Serial.println("Wifi connection successful");
  AWSErrors aws_err = awsManager.init();
  while (aws_err != AWSErrors::OK)
  {
  }
  Serial.println("aws init successful");
  aws_err = awsManager.connect();
  while (aws_err != AWSErrors::OK)
  {
  }
  Serial.println("aws connection successful");
  while (1)
  {
    if (!awsManager.isConnected())
    {
      Serial.println("aws disconnected, reconnecting..");
      awsManager.connect();
    }
    Serial.println("aws task looping");
    vTaskDelay(1000);
  }
}

void publishTask(void *parameter)
{ // global instance
  while (true)
  {
    do
    {
      std::string weight;
      DBErrors err = db_Handler.read_DB_Multiple(10, weight);
      if (err == DBErrors::OK)
      {
        Serial.printf("latest db row is: %s\n", weight.c_str());
        std::vector<uint16_t> ids;
        err = DB::extractIDs(weight, ids);
        if (err == DBErrors::OK)
        {
          Serial.println("Id extraction OK");
          // std::string dummy = "Hello wolrd";
          AWSErrors aws_err = awsManager.publish(publish_topic, weight);
          if (aws_err != AWSErrors::OK)
          {
            Serial.println("AWS publish failed");
            break;
          }
          err = db_Handler.delete_DB_Multiple(ids);
          if (err == DBErrors::OK)
          {
            Serial.println("Deletion successful");
          }
          else
          {
            Serial.println("Deletion failed");
          }
          deleteBool = true;
        }
      }
      else
      {
        Serial.printf("Error reading Db %d\n", (int)err);
      }
    } while (0);

    // Adjust the delay according to your requirements
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void databaseTask(void *parameter)
{
  if (db_Handler.create_DB() != DBErrors::OK)
  {
    Serial.println("database creation/open failed...Aplication failure");
    exit(1);
  }
  DBErrors err = db_Handler.create_DB_Table();
  if (err != DBErrors::OK)
  {
    Serial.println("database table create/open...Aplication failure");
    Serial.printf("error code: %d", (int)err);
    exit(1);
  }
  Serial.println("starting db task");
  while (true)
  {
    float weight;
    std::string timestamp;
    Serial.println("Waiting for data on the queue");
    if (xQueueReceive(weightQueue, &weight, pdMS_TO_TICKS(portMAX_DELAY)) == pdTRUE)
    {
      Serial.printf("received from queue %.2f\n", weight);
      std::string timestamp = timing.sendLocalTime();
      db_Handler.insert_DB(weight, timestamp);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
    Serial.println("looping db task");
  }
}

void setup()
{
  Serial.begin(115200);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  Serial.println("Weight of Gas Loading");
  weightQueue = xQueueCreate(QUEUE_SIZE, sizeof(float));
  sdCardManager.initSDCard();
  myScale.begin();
  Serial.printf("\nHeap size: %d\n", ESP.getHeapSize());
  Serial.printf("Free Heap: %d\n", esp_get_free_heap_size());
  Serial.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
  Serial.printf("Max Alloc Heap: %d\n", ESP.getMaxAllocHeap());
  Serial.println("starting tasks");
  Serial.println("aws publish successful");
  xTaskCreatePinnedToCore(scaleTask, "Scale Task", 2048, NULL, 2, &scaleTaskHandle, APP_CPU_NUM);
  xTaskCreatePinnedToCore(publishTask, "Publish Task", 4096, NULL, 1, &publishTaskHandle, APP_CPU_NUM);
  xTaskCreatePinnedToCore(databaseTask, "Database Task", 4096, NULL, 2, NULL, APP_CPU_NUM);
  xTaskCreatePinnedToCore(connectivityTask, "connectivityTask Task", 8192, NULL, 1, NULL, PRO_CPU_NUM);
}

void loop()
{
  // This loop will not be used as tasks have been created.
  //    float weight = db_Handler.getLatestWeight();
  //    awsManager.publishMessage(weight);
  //    awsManager.connectAWS();
  //    awsManager.client.loop();
  //  delay(1000);
  //   AsyncElegantOTA.loop();
  delay(1000);
}
