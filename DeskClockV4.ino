#include <WiFi.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // 5 hours 30 minutes in seconds
const int daylightOffset_sec = 0;  // No daylight saving time in India

//const char* ssid = "Norway";
//const char* password = "EmmaVnyv10";

const char* ssid = "Praveen_Jio";
const char* password = "Praveen@123";

String text = "";

struct tm timeinfo;
bool timeUpdateFlag = 1;



void setup() {
  Serial.begin(115200);  // put your setup code here, to run once:
  Serial.println(WiFi.localIP());

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timeinfo = findLocalTime();

  displayInitialisation();
  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //Serial.println("Wifi disconnected");
  //WiFi.mode(WIFI_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timeUpdateFlag == 1 ) {
      displayTime(timeinfo.tm_hour,
                  timeinfo.tm_min,
                  timeinfo.tm_mday,
                  timeinfo.tm_mon + 1,  // Months are 0-based
                  timeinfo.tm_year + 1900,
                  timeinfo.tm_wday);
      timeUpdateFlag = 0;
    }
  timeinfo = updateTime(timeinfo);
}
