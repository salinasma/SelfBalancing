#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define RIGHT_PWM1 26
#define RIGHT_PWM2 27
#define RIGHT_TACH1 18
#define RIGHT_TACH2 19
#define RIGHT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define LEFT_PWM1 25
#define LEFT_PWM2 33
#define LEFT_TACH1 34
#define LEFT_TACH2 35
#define LEFT_TACH_SAMPLE_INTERVAL 1000 // microseconds

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);

Imu imu (32);

WiFiMulti wifiMulti;
HTTPClient http;

int lPeriod;
int lFrequency = 0;
int lPwm = 0;

int rPeriod;
int rFrequency = 0;
int rPwm = 0;

void setup () {
    Serial.begin (9600);
    while (!Serial) {;}

    // Input your ssid and password here
    wifiMulti.addAP ("ssid", "password"); 
    
    left.rotate (70);
    right.rotate (70);
}

String payload, runStr;
int runVal;

void loop () {
    // wait for WiFi connection
    if ((wifiMulti.run () == WL_CONNECTED)) {
        Serial.print ("[HTTP] begin...\n");
        // input your server's ip address here
        http.begin ("ip address", 8000, "/");
        
        Serial.print ("[HTTP] POST...\n");
        http.addHeader ("Content-Type", "application/json");
        
        int httpCode = http.POST ("{\n  \"function\":\"get_params\",\n  \"args\":{\"searchString\":\"Black\"}\n}");

        // httpCode will be negative on error
        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);

                runStr = payload.substring (payload.indexOf("running\": ") + 10, payload.indexOf (","));
                runVal = runStr.toInt();

                if (runVal) {
                  Serial.println("Running motors");
                  left.rotate (80);
                  right.rotate (80);
                } else {
                  Serial.println("Stopping motors");
                  left.rotate (5);
                  right.rotate (5);
                }
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end ();
    }

    delay (1000);
}
