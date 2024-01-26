#include <Arduino.h>
#include "DB_Handler.h"
#include <sqlite3.h>


static int insert_callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

int static multiplecallback(void *data, int argc, char **argv, char **azColName)
{
    Serial.println("Callback fired");
    //float *weight = static_cast<float *>(data);
    std::string weight_string; 
    weight_string = "{";
    for (int i = 0; i <argc; i++){
        static char temp[50];
        sprintf(temp,"\"%s\":\"%s\"%s",azColName[i],argv[i] ? argv[i] : "NULL",(i==(argc-1)?"":","));
        weight_string+=temp;
    }
    weight_string+="},";
    Serial.printf("final string is: %s\n",weight_string.c_str());
    *(std::string*)data = *(std::string*)data + weight_string;
    return 0;
}

int DB::callback(void *data, int argc, char **argv, char **azColName)
{
    Serial.println("Callback fired");
    //float *weight = static_cast<float *>(data);
    std::string weight_string; 
    weight_string = "{";
    for (int i = 0; i <argc; i++){
        static char temp[50];
        sprintf(temp,"\"%s\":\"%s\"%s",azColName[i],argv[i] ? argv[i] : "NULL",(i==(argc-1)?"":","));
        weight_string+=temp;
    }
    weight_string+="}";
    Serial.printf("final string is: %s\n",weight_string.c_str());
    *(std::string*)data = weight_string;
    return 0;
}
DB::DB(std::string file_path,std::string table_name):file_path(file_path),table_name(table_name)
{
    db = nullptr;
    errMsg = nullptr;
}

DBErrors DB::open(std::string file_path){
    if(dbOpened)return DBErrors::OK;
    if(file_path.empty())
    {
        Serial.println("Cannot open with empty filepath");
        return DBErrors::NULL_PARAMETER_ERROR;
    }
    int rc = sqlite3_open(file_path.c_str(), &db);
    if (rc)
    {
        Serial.println("Can't open database");
        return DBErrors::DB_OPEN_FAILED;
    }
    else
    {
        Serial.println("Opened database successfully");
        dbOpened = true;
        return DBErrors::OK;
    }  
}

DBErrors DB::close(){
    if(dbOpened){
        int rc = sqlite3_close(db); 
        if (rc)
        {
            Serial.println("Can't close database");
            return DBErrors::DB_CLOSE_FAILED;
        }
        return DBErrors::OK;
    }
    else{
        return DBErrors::DB_NOT_OPENED;
    }
}

 DBErrors DB::read_DB_Multiple(uint8_t number_of_records, std::string& db_row)
 {
     if(dbOpened){
        if(!number_of_records)
        {
            return DBErrors::NULL_PARAMETER_ERROR;
        }

        Serial.println("Database opened");
            static char sql[150];
            snprintf(sql,150, "SELECT * FROM %s  ORDER BY id ASC LIMIT %d;", table_name.c_str(),number_of_records);
            Serial.printf("query is: %s\n",sql);
            char *msg = 0;
            // Serial.printf("\nHeap size: %d\n", ESP.getHeapSize());
            // Serial.printf("Free Heap: %d\n", esp_get_free_heap_size());
            // Serial.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
            // Serial.printf("Max Alloc Heap: %d\n", ESP.getMaxAllocHeap());
            db_row ="{\"data\":[";
            if(xSemaphoreTake(dbMutex,pdMS_TO_TICKS(portMAX_DELAY))){
                int rc = sqlite3_exec(db,sql,multiplecallback, &db_row, &msg);

                if (rc != SQLITE_OK)
                {
                    Serial.printf("SQL error: %s \n ", msg);
                    sqlite3_free(msg);
                    xSemaphoreGive(dbMutex);
                    return DBErrors::DATA_READ_FAILED;
                    // Serial.println(sqlite3_errmsg(db));
                }
                xSemaphoreGive(dbMutex);
            }
            db_row[db_row.length()-1] = ']';
            db_row += "}";
            return DBErrors::OK;

            
    }
    else{
        return DBErrors::DB_NOT_OPENED;
    }   
 }

 DBErrors DB::delete_DB_Multiple(std::vector<uint16_t> ids){
 if(dbOpened){
        if(ids.empty())
        {
            return DBErrors::NULL_PARAMETER_ERROR;
        }

        Serial.println("Database opened");
            static char sql[100];
            snprintf(sql,150, "DELETE FROM %s WHERE id IN (", table_name.c_str());
            Serial.printf("query is: %s\n",sql);
            std::string complete_query = sql;
            for(auto id: ids){
                complete_query+=std::to_string(id);
                complete_query+=",";
            }
            complete_query[complete_query.length()-1]=')';
            complete_query+=";";
            Serial.printf("complete query is: %s\n",complete_query.c_str());
            char *msg = 0;
            if(xSemaphoreTake(dbMutex,pdMS_TO_TICKS(portMAX_DELAY))){
                int rc = sqlite3_exec(db,complete_query.c_str(),NULL, NULL, &msg);
                if (rc != SQLITE_OK)
                {
                    Serial.printf("SQL error: %s \n ", msg);
                    sqlite3_free(msg);
                    xSemaphoreGive(dbMutex);
                    return DBErrors::DATA_READ_FAILED;
                    // Serial.println(sqlite3_errmsg(db));
                }
                xSemaphoreGive(dbMutex);
            }


            return DBErrors::OK;         
    }
    else{
        return DBErrors::DB_NOT_OPENED;
    }   
 }

DBErrors  DB::read_DB_Latest(std::string& db_row){
    if(dbOpened){
        Serial.println("Database opened");
            static char sql[150];
            snprintf(sql,150, "SELECT * FROM %s  ORDER BY id DESC LIMIT 1;", table_name.c_str());
            Serial.printf("query is: %s\n",sql);
            char *msg = 0;
            // Serial.printf("\nHeap size: %d\n", ESP.getHeapSize());
            // Serial.printf("Free Heap: %d\n", esp_get_free_heap_size());
            // Serial.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
            // Serial.printf("Max Alloc Heap: %d\n", ESP.getMaxAllocHeap());

            int rc = sqlite3_exec(db,sql, callback, &db_row, &msg);

            if (rc != SQLITE_OK)
            {
                Serial.printf("SQL error: %s \n ", msg);
                sqlite3_free(msg);
                return DBErrors::DATA_READ_FAILED;
                // Serial.println(sqlite3_errmsg(db));
            }
            
            return DBErrors::OK;

            
    }
    else{
        return DBErrors::DB_NOT_OPENED;
    }
}

DBErrors DB::create_DB()
{
    return open(file_path);
}
DBErrors DB::insert_DB(float weight,std::string timestamp){
    if(dbOpened){
        Serial.println("Database opened");
            static char sql[150];
            snprintf(sql,150, "INSERT INTO %s (weight,timestamp) VALUES (%.2f,'%s');", table_name.c_str() ,weight, timestamp.c_str());
            Serial.printf("query is: %s\n",sql);
            const char* message = "insertion called";
            if(xSemaphoreTake(dbMutex,pdMS_TO_TICKS(portMAX_DELAY))){
                int rc = sqlite3_exec(db, sql, insert_callback, (void*)message, &errMsg);
                if (rc != SQLITE_OK)
                {
                    Serial.print("SQL error: ");
                    Serial.println(errMsg);
                    sqlite3_free(errMsg);
                    xSemaphoreGive(dbMutex);
                    return DBErrors::DATA_INSERTION_FAILED;
                }
                else
                {
                    Serial.println("Record inserted successfully");
                    xSemaphoreGive(dbMutex);
                    return DBErrors::OK;
                    // incrementUploadCounter
                    // incrementUploadCounter();
                }    
            }
    }
    else{
        return DBErrors::DB_NOT_OPENED;
    }
}
DBErrors DB::create_DB_Table()
{
    if(table_name.empty())return DBErrors::NULL_PARAMETER_ERROR;
    if(dbOpened){
        static char sql[150];
        snprintf(sql,150,"CREATE TABLE IF NOT EXISTS %s (id INTEGER PRIMARY KEY, weight REAL,timestamp TEXT);",table_name.c_str());
        Serial.printf("query is: %s\n",sql);
        int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK)
        {
            Serial.print("SQL error: ");
            Serial.println(errMsg);
            sqlite3_free(errMsg);
            Serial.println("Table creation failed");
            return DBErrors::TABLE_CRAETION_FAILED;
        }
        else
        {
            Serial.println("Table created successfully");
            return DBErrors::OK ;
        }
    }
    else{
        Serial.println("DB not opened");
        return DBErrors::DB_NOT_OPENED;
    }

}

DBErrors DB::extractIDs(std::string& data, std::vector<uint16_t>& id_vector) {

        const uint16_t JSON_OBJECT_SIZE = 512;
        DynamicJsonDocument id_json(JSON_OBJECT_SIZE);
        if(deserializeJson(id_json, data)){
            Serial.println("Json not parsed corrctly");
            return DBErrors::JSON_ERROR;
        }
        JsonArray array = id_json["data"].as<JsonArray>();
        for(JsonVariant v : array) {
            if (v.containsKey("id")){
                const char* id_ = v["id"];
                Serial.println("Converted string value is " + (String) id_);
                uint16_t id_int = std::atoi (id_);
                Serial.println("Casted variable is " + (String) id_int);
                id_vector.push_back(id_int);
                Serial.println("id vector is" + (String) id_vector[id_vector.size()-1]);
            }
        }  
        return DBErrors::OK;     
     
        
    
}

DBErrors DB::open_DB(){
    return open(file_path);
}

DBErrors DB::close_DB(){
    return close();
}
