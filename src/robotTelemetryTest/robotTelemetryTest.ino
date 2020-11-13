#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>

#define NUM_PARAMS 2
#define NUM_DATA 2
#define JSON_BUFFER_SIZE 500
#define IS_PARAM 1

WiFiMulti wifiMulti;
HTTPClient http;

enum paramType {
  INT = 0,
  CHAR = 1,
  BOOL = 2
};

typedef struct DataObject{
  String dataName;
  void *dataPtr;
  int dataRegIndex;
  int dataType;
  int dataSize;
} Data;

// Example variables
int gobble = 42;
char dataDump[500] = "23";

Data paramRegistry[NUM_PARAMS] = {
{String("gobble"), &gobble, 0, INT, sizeof(gobble)},
{String("dataDump500"), &dataDump, 1, CHAR, 500}
};

Data dataRegistry[NUM_DATA] = {
{String("gobble"), &gobble, 0, INT, sizeof(gobble)},
{String("dataDump500"), &dataDump, 1, CHAR, 500}
};

String jsonInitRegistry (int regSize, Data *registry, bool isParam) {
  String jsonCmd = "{\"function\":\"botInitData\",\"numData\":\"" + (String)regSize + "\",";
  String regType = "d";

  if (isParam) {
    jsonCmd = "{\"function\":\"botInitParams\",\"numParams\":\"" + (String)regSize + "\",";
    regType = "p";
  }
  
  for (int i = 0; i < regSize; i++) {
    jsonCmd += " \"p" + (String)i + "\":";
    jsonCmd += "{\"name\":\"" + (String)registry[i].dataName;
    jsonCmd += "\", \"value\":\"" + (String)(*((int *)(registry[i].dataPtr)));
    jsonCmd += "\", \"index\":\"" + (String)registry[i].dataRegIndex;
    jsonCmd += "\", \"type\":\"" + (String)registry[i].dataType;
    jsonCmd += "\", \"size\":\"" + (String)registry[i].dataSize;
    jsonCmd += "\"}";

    if (i < regSize - 1) {
      jsonCmd += ", ";
    }
  }
    
  jsonCmd += "}";

  return jsonCmd;
}

String jsonUpdateData (Data d) {
  String jsonCmd = "{\"function\":\"botUpdateData\",";
  
  jsonCmd += "\"name\":\"" + (String)d.dataName;
  jsonCmd += "\", \"value\":\"" + (String)(*((int *)(d.dataPtr)));
  jsonCmd += "\"}"; 

  return jsonCmd;
}

String jsonGetParam (Data param) {
  String jsonCmd("{\"function\":\"botGetParam\",");
  
  jsonCmd += "\"name\":\"" + (String)param.dataName;
  jsonCmd += "\"}"; 

  return jsonCmd;
}

String jsonGetParams () {
  String jsonCmd("{\"function\":\"botGetParams\"}");

  return jsonCmd;
}

String jsonRxCmd () {
  String jsonCmd("{\"function\":\"botRxCmd\"}");

  return jsonCmd;
}

void setup () {
    int httpCode;
    
    Serial.begin (9600);
    while (!Serial);
    
    // Input your ssid and password here
    wifiMulti.addAP ("ssid", "password"); 

    // Init the server
    while (wifiMulti.run() != WL_CONNECTED);
    http.begin ("server ip address", 8000, "/");
    
    do {
      http.addHeader ("Content-Type", "application/json");
      httpCode = http.POST (jsonInitRegistry (NUM_PARAMS, paramRegistry, IS_PARAM));
    } while (httpCode == 0);

    do {
      http.addHeader ("Content-Type", "application/json");
      httpCode = http.POST (jsonInitRegistry (NUM_DATA, dataRegistry, !IS_PARAM));
    } while (httpCode == 0);
}
      
StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
String payload;
void loop () {
    // wait for WiFi connection
    if ((wifiMulti.run () == WL_CONNECTED)) {
        Serial.print ("[HTTP] begin...\n"); 
        Serial.print ("[HTTP] POST...\n");
        
        http.addHeader ("Content-Type", "application/json");
        String jsonUpdateCmd = jsonUpdateData (dataRegistry[0]);
        int httpCode = http.POST (jsonUpdateCmd);
        Serial.println (jsonUpdateCmd);

        // httpCode will be negative on error
        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.addHeader ("Content-Type", "application/json");
        String jsonGetParamsCmd = jsonGetParams ();
        httpCode = http.POST (jsonGetParamsCmd);
        Serial.println (jsonGetParamsCmd);

        // httpCode will be negative on error
        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                Serial.println(payload);
            }
            
            JsonObject& root = jsonBuffer.parseObject(payload);
            while (!root.success()){
              JsonObject& root = jsonBuffer.parseObject(payload);
            } 
            // using C++98 syntax (for older compilers):
            for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
              Serial.println(it->key);
              Serial.println(it->value.as<char*>());
              int index = String(it->key).toInt();
              int value = String((it->value).as<char *>()).toInt();
              *((int *)(paramRegistry[index].dataPtr)) = value;
            }
            jsonBuffer.clear();
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.addHeader ("Content-Type", "application/json");
        String jsonUpdateParamsCmd = jsonInitRegistry (NUM_PARAMS, paramRegistry, IS_PARAM);
        httpCode = http.POST (jsonUpdateParamsCmd);
        Serial.println (jsonUpdateParamsCmd);

        // httpCode will be negative on error
        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
//        http.end ();
    }
    
    gobble++;
    delay (1000);
}
