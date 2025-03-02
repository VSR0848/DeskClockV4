#include "time.h"
#include <WiFi.h>
#include "FS.h"
#include "LittleFS.h"      // Use LittleFS instead of SPIFFS
#include "esp_task_wdt.h"  // Include watchdog library
#define WDT_TIMEOUT 10     // Set watchdog timeout (in seconds)


const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // 5 hours 30 minutes in seconds
const int daylightOffset_sec = 0;  // No daylight saving time in India

//const char* ssid = "Norway";
//const char* password = "EmmaVnyv10";

//const char* ssid = "Praveen_Jio";`
//const char* password = "Praveen@123";

//const char* ssid = "NotFree";
//const char* password = "lazyaf0848";

String text = "";

struct tm timeinfo;
bool timeUpdateFlag = 1;



void setup() {
  Serial.begin(115200);  // put your setup code here, to run once:
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = WDT_TIMEOUT * 1000,  // Convert seconds to milliseconds
    .idle_core_mask = (1 << 0),        // Watch Core 0
    .trigger_panic = true              // Cause a panic/reset on timeout
  };
  // Initialize the Watchdog Timer with the config struct
  esp_task_wdt_init(&wdt_config);
  // Register the current task (optional but recommended)
  esp_task_wdt_add(NULL);  // Add current task to watchdog

  displayInitialisation();
  bootingtextAnimation("display connected...");
  if (!LittleFS.begin(true)) {  // true → format on failure
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("SPIFFS Mounted Successfully");
  bootingtextAnimation("SPIFFS Mounted Successfully...");
  bootingtextAnimation("Connectig to Wifi...");
  scanAndConnectWiFi();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed. Restarting...");
    bootingtextAnimation("WiFi connection failed...");
    ESP.restart();  // Restart ESP32 if no connection
  }
  bootingtextAnimation("Connecting to Time server...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timeinfo = findLocalTime();
  bootingtextAnimation("Time Obtained...");
  delay(300);
  bootingtextAnimation("Syncing time...");

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //Serial.println("Wifi disconnected");
  //WiFi.mode(WIFI_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:
  esp_task_wdt_reset();  // Reset WDT to prevent timeout
  if (timeUpdateFlag == 1) {
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
