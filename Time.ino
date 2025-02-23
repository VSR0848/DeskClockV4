
int timeTextCenterYPos = 32;
unsigned long lastTimeUpdate = 0;  // Track last time update
const unsigned long updateInterval = 60000; // 60 seconds in milliseconds


void displayTime(int hr, int min, int day, int month, int year, int wday) {
  String hrStr = String(hr);
  String minStr = String(min);
  Serial.println(hrStr);
  Serial.println(minStr);

  if (hr < 10) hrStr = "0" + hrStr;     // Add leading zero for single-digit hour
  if (min < 10) minStr = "0" + minStr;  // Add leading zero for single-digit minute

  String time = hrStr + ":" + minStr;
  Serial.println(time);


  String dayStr = String(day);
  String monthStr = String(month);
  String yearStr = String(year);
  String dayofWeek = getDayOfWeek(wday);
  String date = dayStr + "." + monthStr + "." + yearStr + "  " + dayofWeek;

  String previousDate = date;
}

struct tm findLocalTime() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    return timeinfo;
  } else {
    Serial.println("Failed to obtain time");
    //struct tm defaultTime = {};
    //return ;
    findLocalTime();
  }
  return timeinfo;
}

String getDayOfWeek(int day) {
  switch (day) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    default: return "Invalid day";
  }
}

struct tm updateTime(struct tm currentTime) {
  unsigned long currentMillis = millis();

  // Check if 60 seconds have passed
  if (currentMillis - lastTimeUpdate >= updateInterval) {
    lastTimeUpdate = currentMillis;  // Reset the last update time

    // Increment the time (currentTime is the tm structure)
    currentTime.tm_sec += 60;  // Increment seconds by 60 to represent 1 minute

    // Handle overflow of seconds into minutes, hours, and days
    if (currentTime.tm_sec >= 60) {
      currentTime.tm_sec = 0;
      currentTime.tm_min++;  // Increment minute
    }

    if (currentTime.tm_min >= 60) {
      currentTime.tm_min = 0;
      currentTime.tm_hour++;  // Increment hour
    }

    if (currentTime.tm_hour >= 24) {
      currentTime.tm_hour = 0;
      currentTime.tm_mday++;  // Increment day
    }

    int daysInMonth = getDaysInMonth(currentTime.tm_mon, currentTime.tm_year);
    
    if (currentTime.tm_mday > daysInMonth) {
      currentTime.tm_mday = 1;  // Reset to 1st day of the month
      currentTime.tm_mon++;     // Increment month
    }

    // Handle year overflow
    if (currentTime.tm_mon >= 12) {
      currentTime.tm_mon = 0;
      currentTime.tm_year++;  // Increment year (years since 1900)
    }
  }

  return currentTime;
}


int getDaysInMonth(int month, int year) {
  // Months are 0-based (0 = January, 11 = December)
  switch (month) {
    case 0: case 2: case 4: case 6: case 7: case 9: case 11:  // 31 days
      return 31;
    case 3: case 5: case 8: case 10:  // 30 days
      return 30;
    case 1:  // February
      return isLeapYear(year) ? 29 : 28;  // Leap year check
    default:
      return 31;  // Default to 31 if month is out of range (shouldn't happen)
  }
}

// Function to check if a year is a leap year
bool isLeapYear(int year) {
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    return true;
  }
  return false;
}
