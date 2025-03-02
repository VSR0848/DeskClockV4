#include <WiFi.h>


#define WIFI_CREDENTIALS_FILE "/wifi_credentials.txt"
int wifiConnectionAttempt = 0;
/*usual code
  Serial.println(WiFi.localIP());

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/

#define MAX_ATTEMPTS 15  // Maximum retry attempts for connecting to WiFi

void scanAndConnectWiFi() {
  Serial.println("Scanning for available networks...");
  bootingtextAnimation("Scanning for available network");

  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == 0) {
    Serial.println("No networks found.");
    bootingtextAnimation("No networks found.");
    return;
  }

  // Read stored credentials
  File file = LittleFS.open(WIFI_CREDENTIALS_FILE, "r");  // Use LittleFS instead of SPIFFS
  if (!file) {
    Serial.println("❌ Failed to open WiFi credentials file");
    bootingtextAnimation("Failed to open WiFi credentials file");
    return;
  }

  String storedSSIDs[10];  // Max 10 SSIDs
  String storedPasswords[10];
  int storedCount = 0;

  // Credential file check
  Serial.println("📂 File Contents:");
  while (file.available()) {
    Serial.write(file.read());  // Print raw file data
  }
  file.seek(0);  // Reset file pointer

  // Read credentials from file
  while (file.available() && storedCount < 10) {
    String line = file.readStringUntil('\n');
    line.trim();
    int sepIndex = line.indexOf(',');
    if (sepIndex != -1) {
      storedSSIDs[storedCount] = line.substring(0, sepIndex);
      storedPasswords[storedCount] = line.substring(sepIndex + 1);
      storedCount++;
    }
  }
  file.close();  // Close the file after reading

  Serial.println("✅ Stored SSIDs:");
  for (int k = 0; k < storedCount; k++) {
    Serial.printf("[%d] SSID: '%s'\n", k, storedSSIDs[k].c_str());  // Print stored SSIDs
  }
  file.close();
  Serial.println("Stored SSIDs:");
  for (int k = 0; k < storedCount; k++) {
    Serial.printf("[%d] SSID: '%s'\n", k, storedSSIDs[k].c_str());  // Print stored SSIDs
  }

  // Compare scanned SSIDs with stored credentials
  for (int i = 0; i < numNetworks; i++) {
    String scannedSSID = WiFi.SSID(i);
    //Serial.printf("Found SSID: %s\n", scannedSSID.c_str());
    for (int j = 0; j < storedCount; j++) {
      //Serial.println(("Stored SSID: %s\n", storedSSIDs[j]);
      Serial.println(scannedSSID);
      Serial.print("==" + storedSSIDs[j]);
      if (scannedSSID == storedSSIDs[j]) {
        Serial.printf("Matching SSID found: %s, connecting...\n", scannedSSID.c_str());
        bootingtextAnimation("Matching SSID found.");
        bootingtextAnimation("Connecting");

        WiFi.begin(storedSSIDs[j].c_str(), storedPasswords[j].c_str());

        int attempt = 0;
        while (WiFi.status() != WL_CONNECTED && attempt < MAX_ATTEMPTS) {
          delay(1000);
          Serial.printf("Attempt %d/%d...\n", attempt + 1, MAX_ATTEMPTS);
          bootingtextAnimation("Attempt " + String(attempt + 1) + "/" + String(MAX_ATTEMPTS));
          attempt++;
        }

        if (WiFi.status() == WL_CONNECTED) {
          Serial.printf("\nConnected to %s\n", storedSSIDs[j].c_str());
          bootingtextAnimation("Connected to " + storedSSIDs[j]);

          Serial.print("IP Address: ");
          Serial.println(WiFi.localIP());
          bootingtextAnimation("IP Address: " + WiFi.localIP().toString());

          return;  // Exit after successful connection
        } else {
          Serial.println("\nConnection failed. Trying next...");
          bootingtextAnimation("Connection failed. Trying next...");
        }
      }
    }
  }

  Serial.println("No matching SSID found.");
  bootingtextAnimation("No matching SSID found.");
}
