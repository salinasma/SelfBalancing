/*
 * Author: Pramuka Perera
 * Description: Telemetery and Control Client
 */

#include "TCClient.h"

TCClient::TCClient(const char *ssid, const char *wifiPassword, const char *serverAddress, Data *paramReg, int paramRegSize, Data *dataReg, int dataRegSize) {
    wifiMulti.addAP(ssid, wifiPassword);
    while (wifiMulti.run() != WL_CONNECTED); 
    http.begin(serverAddress, SERVER_PORT, "/");

    paramRegistry = paramReg;
    dataRegistry = dataReg;

    this->paramRegSize = paramRegSize;
    this->dataRegSize = dataRegSize;
}

// POST Request to push the parameters from the robot to the server and ui
int TCClient::postInitParamRegistry() {
    int httpCode;
    String jsonCmd = "{\"function\":\"botInitParams\",\"numParams\":\"" + (String)paramRegSize + "\",";
  
    for (int i = 0; i < paramRegSize; i++) {
      jsonCmd += " \"p" + String(i) + "\":";
      jsonCmd += "{\"name\":\"" + String(paramRegistry[i].dataName);
      switch (paramRegistry[i].dataType) {
          case INT:
            jsonCmd += "\", \"value\":\"" + String(*((int *)(paramRegistry[i].dataPtr)));
            break;

          case DOUBLE:
            // convert from float to fixed point
            jsonCmd += "\", \"value\":\"" + String((int)((*((double *)(paramRegistry[i].dataPtr))) * FIXED_POINT_CONVERSION_CONSTANT));
            break;

          default:
            jsonCmd += "\", \"value\":\"" + String(-1);
            break;
      };
      jsonCmd += "\", \"index\":\"" + String(paramRegistry[i].dataRegIndex);
      jsonCmd += "\", \"type\":\"" + String(paramRegistry[i].dataType);
      jsonCmd += "\", \"size\":\"" + String(paramRegistry[i].dataSize);
      jsonCmd += "\"}";
  
      if (i < paramRegSize - 1) {
        jsonCmd += ", ";
      }
    }
    jsonCmd += "}";

    Serial.println(jsonCmd);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

// POST Request to push the data from the robot to the server and ui
int TCClient::postInitDataRegistry() {
    int httpCode;
    String jsonCmd = "{\"function\":\"botInitData\",\"numData\":\"" + (String)dataRegSize + "\",";
  
    for (int i = 0; i < dataRegSize; i++) {
      jsonCmd += " \"d" + (String)i + "\":";
      jsonCmd += "{\"name\":\"" + (String)dataRegistry[i].dataName;
      jsonCmd += "\", \"value\":\"" + (String)(*((int *)(dataRegistry[i].dataPtr)));
      jsonCmd += "\", \"index\":\"" + (String)dataRegistry[i].dataRegIndex;
      jsonCmd += "\", \"type\":\"" + (String)dataRegistry[i].dataType;
      jsonCmd += "\", \"size\":\"" + (String)dataRegistry[i].dataSize;
      jsonCmd += "\"}";
  
      if (i < dataRegSize - 1) {
        jsonCmd += ", ";
      }
    }
    jsonCmd += "}";

    Serial.println(jsonCmd);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

// POST request to push new data to the server and ui
int TCClient::postUpdateData(int dataRegIndex) {
    int httpCode;
    Data d = dataRegistry[dataRegIndex];

    String jsonCmd = "{\"function\":\"botUpdateData\",";
    jsonCmd += "\"name\":\"" + String(d.dataName);
    jsonCmd += "\", \"value\":\"" + String(*((int *)(d.dataPtr)));
    jsonCmd += "\"}";
    
    Serial.println(jsonCmd);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

// POST request to get a specific parameter value from the server
int TCClient::postGetParam(int paramRegIndex) {
    int httpCode;
    Data p = paramRegistry[paramRegIndex];

    String jsonCmd("{\"function\":\"botGetParam\",");
    jsonCmd += "\"name\":\"" + (String)p.dataName;
    jsonCmd += "\"}";

    Serial.println(jsonCmd);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

// POST request to get all the parameter values from the server
int TCClient::postGetParams() {
    int httpCode;
    String jsonCmd("{\"function\":\"botGetParams\"}");

    Serial.println(jsonCmd);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(jsonCmd);
    
    return httpCode;
}

// POST request to get a command from the server
int TCClient::postRxCmd() {
    int httpCode;
    String jsonCmd("{\"function\":\"botRxCmd\"}");

    Serial.println(jsonCmd);
    
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
    static StaticJsonDocument<JSON_BUFFER_SIZE> jsonDoc;
    static JsonObject::iterator it;
    String payload;

    // httpCode will be negative on error
    if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    
        if (httpCode == HTTP_CODE_OK) {
            payload = http.getString();
            Serial.println(payload);
        }
        
        deserializeJson(jsonDoc, payload);
        JsonObject res = jsonDoc.as<JsonObject>();

        switch (postCmdId) {
            case INIT_PARAMS:
                break;

            case INIT_DATA:
                break;

            case GET_PARAMS:
                // write new params to param variables
                for (it = res.begin(); it != res.end(); ++it) {
                  int index = String(it->key().c_str()).toInt();
                  int value = it->value().as<signed int>();

                  Serial.println(index + ": " + value);

                  switch (paramRegistry[index].dataType) {
                      case INT:
                        *((int *)(paramRegistry[index].dataPtr)) = value;
                        break;

                      case DOUBLE:
                        // convert from fixed point to float
                        *((double *)(paramRegistry[index].dataPtr)) = double(value) / FIXED_POINT_CONVERSION_CONSTANT;
                        break;

                      default:
                        break;
                  };
                }

                jsonDoc.clear();
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

