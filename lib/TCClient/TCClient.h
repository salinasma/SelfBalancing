/*
 * Author: Pramuka Perera
 * Description: Telemetery and Control Client
 */

#ifndef TCCLIENT_H
#define TCCLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SERVER_PORT 8000
#define JSON_BUFFER_SIZE 500
#define IS_PARAM 1
#define FIXED_POINT_CONVERSION_CONSTANT double(1000.0)

enum paramType{
  INT = 0,
  DOUBLE = 1
};

enum postCmdType{
    INIT_PARAMS = 0,
    INIT_DATA = 1,
    GET_PARAMS = 2,
    GET_SINGLE_PARAM = 3,
    UPDATE_DATA = 4,
    RX_CMD = 5
};

typedef struct DataObject{
  String dataName;
  void *dataPtr;
  int dataRegIndex;
  int dataType;
  int dataSize;
} Data;

class TCClient {
    public:
        TCClient(const char *ssid, const char *wifiPassword, const char *serverAddress, Data *paramReg, int paramRegSize, Data *dataReg, int dataRegSize);
            
        void addParam(String name, void *ptrToParam, int type, int size);  
        
        void addData(String name, void *ptrToData, int type, int size); 
        
        int postInitParamRegistry();

        int postInitDataRegistry();
        
        int postUpdateData(int dataRegIndex); 
        
        int postGetParam(int paramRegIndex); 
        
        int postGetParams(); 
        
        int postRxCmd(); 
        
        bool isConnectedToWifi(); 
        
        void parseJSONResponse(int httpCode, int postCmdId); 

    private:
        WiFiMulti wifiMulti;
        HTTPClient http;

        Data *paramRegistry;
        int paramRegSize;
        
        Data *dataRegistry;
        int dataRegSize;

};

#endif
