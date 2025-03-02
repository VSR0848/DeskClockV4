//#include "Roboto_Bold_12.h"
#include "Roboto_12.h"
#include "Roboto_18.h"
//#include "Roboto_Bold_24.h"
#include "Roboto_Bold_40.h"
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>

#define GxEPD2_DRIVER_CLASS GxEPD2_154_D67  // GDEH0154D67 200x200, SSD1681, (HINK-E154A07-A1)
int bootingTextPosyshift = 0;


// Define display constructor for ESP32 DevKit V1
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(
  GxEPD2_154_D67(/*CS=*/5, /*DC=*/17, /*RST=*/16, /*BUSY=*/4));

void displayInitialisation() {
  Serial.println("Initializing display...");

  SPI.begin(18, 19, 23, 5);  // SCK=18, MISO=19, MOSI=23, CS=5
  Serial.println("SPI Initialized!");

  display.init(115200, true, 2, false);

  display.setRotation(2);
  display.setTextColor(GxEPD_BLACK);
  display.fillScreen(GxEPD_WHITE);
}


void displayTime(int hr, int min, int day, int month, int year, int wday) {

  String time = format12HourTime(hr, min);
  Serial.println(time);

  String dayStr = String(day);
  String monthStr = String(month);
  String yearStr = String(year);
  String dayofWeek = getDayOfWeek(wday);
  String date = dayStr + "." + monthStr + "." + yearStr + "  " + dayofWeek;
  Serial.println(date);

  display.init(115200, true, 2, false);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setFont(&Roboto_Bold_40);
    //display.setTextSize(1);
    int16_t timebx, timeby;
    uint16_t timebw, timebh;
    display.getTextBounds(time, 0, 0, &timebx, &timeby, &timebw, &timebh);
    uint16_t timePosx = ((display.width() - timebw) / 2) - timebx;
    uint16_t timePosy = ((display.height() - timebh) / 9) - timeby;
    display.setCursor(timePosx, timePosy);
    display.print(time);


    display.setFont(&Roboto_18);
    //display.setTextSize(1);
    int16_t datebx, dateby;
    uint16_t datebw, datebh;
    display.getTextBounds(date, 0, 0, &datebx, &dateby, &datebw, &datebh);
    uint16_t datePosx = (((display.width() - datebw) / 2));
    uint16_t datePosy = timePosy + timebh + 2;
    display.setCursor(datePosx, datePosy);
    display.print(date);

    display.drawLine(0 + 20, datePosy + 10, display.width() - 20, datePosy + 10, GxEPD_BLACK);
  } while (display.nextPage());

  String previousDate = date;
  //display.hibernate();
}



void bootingtextAnimation(String text) {
  display.setFont(&Roboto_12);
  int16_t textbx, textby;
  uint16_t textbw, textbh;
  
  // Get text dimensions
  display.getTextBounds(text, 0, bootingTextPosyshift, &textbx, &textby, &textbw, &textbh);
  
  // Calculate position
  uint16_t bootingTextPosx = 0;
  uint16_t bootingTextPosy = ((display.height() - textbh)) - textby; 

  // Set cursor to new line position
  display.setCursor(bootingTextPosx, bootingTextPosy);

  // Print without clearing the screen
  display.print(text);
  
  // Update display without clearing previous text
  display.display();  

  // Move cursor for next line
  bootingTextPosyshift += textbh + 4;  // Adjust spacing for next line
}
