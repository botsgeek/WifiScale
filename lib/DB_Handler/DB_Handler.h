#ifndef DH_HANDLER_H
#define DH_HANDLER_H
#include <sqlite3.h>
#include "timing.h"
#include "freertos/queue.h"
#include <vector>
#include <ArduinoJson.h>

#define QUEUE_SIZE 10
extern QueueHandle_t weightQueue;
enum class DBErrors{
    OK,
    DB_NOT_OPENED,
    DB_OPEN_FAILED,
    TABLE_CRAETION_FAILED,
    DATA_INSERTION_FAILED,
    DATA_READ_FAILED,
    DB_CLOSE_FAILED,
    NULL_PARAMETER_ERROR,
    JSON_ERROR
};
//static int callback(void *data, int argc, char **argv, char **azColName);
class DB {
public:
    DB(std::string file_path,std::string table_name);
    DBErrors create_DB();
    DBErrors create_DB_Table();
    DBErrors open_DB();
    DBErrors insert_DB(float weight,std::string timestamp);
    DBErrors read_DB_Latest(std::string& db_row);
    DBErrors read_DB_Multiple(uint8_t number_of_records, std::string& db_row);
    DBErrors delete_DB_Multiple(std::vector<uint16_t> ids);
    static DBErrors extractIDs(std::string& data,std::vector<uint16_t>& id_vector);
    DBErrors close_DB();
   
    
    // float getLatestWeight();
    //edited
    //void incrementUploadCounter();
private:
    sqlite3* db;
    char* errMsg;    
    int uploadCounter;  // Counter to track the number of records uploaded
    static int callback(void* data, int argc, char** argv, char** azColName);
    SemaphoreHandle_t dbMutex = xSemaphoreCreateMutex();
    DBErrors open(std::string file_path);
    bool dbOpened = false;
    DBErrors close();
    std::string file_path;
    std::string table_name;
};

#endif



