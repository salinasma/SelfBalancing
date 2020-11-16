/*
 * Author: Pramuka Perera
 * Description: Telemetery and Control Client
 */

#include "TCClient.h"

TCClient::TCClient(const char *ssid, const char *wifiPassword, const char *serverAddress, Data *paramReg, Data *dataReg) {
    wifiMulti.addAP(ssid, wifiPassword);
    while (wifiMulti.run() != WL_CONNECTED); 
    http.begin(serverAddress, SERVER_PORT, "/");

    paramRegistry = paramReg;
    dataRegistry = dataReg;
}

void TCClient::addParam(String name, void *ptrToParam, int type, int size) { 
    if (paramRegIndex > NUM_PARAMS) {
        return;
    }
    
    paramRegistry[paramRegIndex].dataName = name;
    paramRegistry[paramRegIndex].dataPtr = ptrToParam;
    paramRegistry[paramRegIndex].dataRegIndex = paramRegIndex;
    paramRegistry[paramRegIndex].dataType = type;
    paramRegistry[paramRegIndex].dataSize = size;

    paramRegIndex++;

    return;
}   

void TCClient::addData(String name, void *ptrToData, int type, int size) { 
    if (dataRegIndex > NUM_DATA) {
        return;
    }

    dataRegistry[dataRegIndex].dataName = name;
    dataRegistry[dataRegIndex].dataPtr = ptrToData;
    dataRegistry[dataRegIndex].dataRegIndex = dataRegIndex;
    dataRegistry[dataRegIndex].dataType = type;
    dataRegistry[dataRegIndex].dataSize = size;

    dataRegIndex++;

    return;
}   

int TCClient::postInitParamRegistry(int numParams) {
    int httpCode;
    String jsonCmd = "{\"function\":\"botInitParams\",\"numParams\":\"" + (String)numParams + "\",";
  
    for (int i = 0; i < numParams; i++) {
      jsonCmd += " \"p" + (String)i + "\":";
      jsonCmd += "{\"name\":\"" + (String)paramRegistry[i].dataName;
      jsonCmd += "\", \"value\":\"" + (String)(*((int *)(paramRegistry[i].dataPtr)));
      jsonCmd += "\", \"index\":\"" + (String)paramRegistry[i].dataRegIndex;
      jsonCmd += "\", \"type\":\"" + (String)paramRegistry[i].dataType;
      jsonCmd += "\", \"size\":\"" + (String)paramRegistry[i].dataSize;
      jsonCmd += "\"}";
  
      if (i < numParams - 1) {
        jsonCmd += ", ";
      }
    }
    jsonCmd += "}";

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

int TCClient::postInitDataRegistry(int numData) {
    int httpCode;
    String jsonCmd = "{\"function\":\"botInitData\",\"numData\":\"" + (String)numData + "\",";
  
    for (int i = 0; i < numData; i++) {
      jsonCmd += " \"d" + (String)i + "\":";
      jsonCmd += "{\"name\":\"" + (String)dataRegistry[i].dataName;
      jsonCmd += "\", \"value\":\"" + (String)(*((int *)(dataRegistry[i].dataPtr)));
      jsonCmd += "\", \"index\":\"" + (String)dataRegistry[i].dataRegIndex;
      jsonCmd += "\", \"type\":\"" + (String)dataRegistry[i].dataType;
      jsonCmd += "\", \"size\":\"" + (String)dataRegistry[i].dataSize;
      jsonCmd += "\"}";
  
      if (i < numData - 1) {
        jsonCmd += ", ";
      }
    }
    jsonCmd += "}";

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

int TCClient::postUpdateData(int dataRegIndex) {
    int httpCode;
    Data d = dataRegistry[dataRegIndex];

    String jsonCmd = "{\"function\":\"botUpdateData\",";
    jsonCmd += "\"name\":\"" + (String)d.dataName;
    jsonCmd += "\", \"value\":\"" + (String)(*((int *)(d.dataPtr)));
    jsonCmd += "\"}";

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

int TCClient::postGetParam(int paramRegIndex) {
    int httpCode;
    Data param = dataRegistry[dataRegIndex];

    String jsonCmd("{\"function\":\"botGetParam\",");
    jsonCmd += "\"name\":\"" + (String)param.dataName;
    jsonCmd += "\"}";

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

int TCClient::postGetParams() {
    int httpCode;
    String jsonCmd("{\"function\":\"botGetParams\"}");

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

int TCClient::postRxCmd() {
    int httpCode;
    String jsonCmd("{\"function\":\"botRxCmd\"}");
    
    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

bool TCClient::isConnectedToWifi() {
    if ((wifiMulti.run() == WL_CONNECTED)) {
        return true;
    }

    return false;
}

void TCClient::parseJSONResponse(int httpCode, int postCmdId) {
    static StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
    JsonObject::iterator it;
    String payload;

    // httpCode will be negative on error
    if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
            payload = http.getString();
            Serial.println(payload);
        }
        JsonObject& root = jsonBuffer.parseObject(payload);

        switch (postCmdId) {
            case INIT_PARAMS:
                break;

            case INIT_DATA:
                break;

            case GET_PARAMS:

                // write new params to param variables
                for (it = root.begin(); it != root.end(); ++it) {
                  Serial.println(it->key);
                  Serial.println(it->value.as<char*>());

                  int index = String(it->key).toInt();
                  int value = String((it->value).as<char *>()).toInt();

                  *((int *)(paramRegistry[index].dataPtr)) = value;
                }

                jsonBuffer.clear();
                break;

            case UPDATE_DATA:
                break;

            case RX_CMD:
                break;

            default:
                break;
        };

    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
}

