#include <TCClient.h>

int gobble = 0;
int bobble = 255;

int httpCode;

TCClient *botClient;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  botClient = new TCClient("ssid", "password", "server address");
//  botClient->addParam(String("bobble"), &bobble, INT, sizeof(bobble));
//  botClient->addData(String("gobble"), &gobble, INT, sizeof(gobble));

//  do {
//    httpCode = botClient->postInitParamRegistry();
//  } while (httpCode == 0);
//
//  do {
//    httpCode = botClient->postInitDataRegistry();
//  } while (httpCode == 0);
}

void loop() {
  if (botClient->isConnectedToWifi()) {
    Serial.println("Sending new data");
//    httpCode = botClient->postUpdateData(0);
//    botClient->parseJSONResponse(httpCode, UPDATE_DATA);
//
//    Serial.println("Getting new params");
//    httpCode = botClient->postGetParams();
//    botClient->parseJSONResponse(httpCode, GET_PARAMS);
  }
  
  gobble++;
  delay(500);
}
