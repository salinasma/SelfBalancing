#include <TCClient.h>

#define NUM_PARAMS 1
#define NUM_DATA 1

int gobble = 0;
int bobble = 255;

int httpCode;

TCClient *botClient;

Data paramReg[NUM_PARAMS] = {{"bobble", &bobble, 0, INT, sizeof(bobble)}};
Data dataReg[NUM_DATA] = {{"gobble", &gobble, 0, INT, sizeof(gobble)}};

void setup() {
  Serial.begin(9600);
  while (!Serial);

  botClient = new TCClient("ssid", "pass", "serverAddress", paramReg, dataReg);

  do {
    httpCode = botClient->postInitParamRegistry(NUM_PARAMS);
  } while (httpCode == 0);

  do {
    httpCode = botClient->postInitDataRegistry(NUM_DATA);
  } while (httpCode == 0);
}

void loop() {
  if (botClient->isConnectedToWifi()) {
    Serial.println("Sending new data");
    httpCode = botClient->postUpdateData(0);
    botClient->parseJSONResponse(httpCode, UPDATE_DATA);

    Serial.println("Getting new params");
    httpCode = botClient->postGetParams();
    botClient->parseJSONResponse(httpCode, GET_PARAMS);
    
    Serial.println("Sending new params");
    httpCode = botClient->postInitParamRegistry(NUM_PARAMS);
    botClient->parseJSONResponse(httpCode, INIT_PARAMS);
  }
  
  gobble++;
  delay(500);
}
