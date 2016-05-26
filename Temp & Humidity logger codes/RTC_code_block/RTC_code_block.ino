#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;


void setup () {
  Serial.begin(9600);
  
  
  Wire.begin();
  RTC.begin();
  Serial.println("rtc starting");
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  Serial.println("rtc booted");
  
  
}

void loop () {
    DateTime now = RTC.now();
    String date = "";
    date += now.month();
    date += "/";
    date += now.day();
    date += "/";
    date += now.year();
    
    String time = "";
    time += now.hour();
    time += ":";
    time += now.minute();
    time += ":";
    time += now.second();
    
    Serial.println(date);
    Serial.println(time);
    delay(3000);
    
}
