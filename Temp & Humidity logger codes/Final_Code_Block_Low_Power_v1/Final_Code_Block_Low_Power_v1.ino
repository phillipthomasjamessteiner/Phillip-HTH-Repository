#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

#include <Wire.h>
#include "RTClib.h"

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

File dataFile;

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 RTC;

double temp = 0; // Celcius
double hum = 0; // humidity

boolean bState = false;
boolean bBounce = false;

int bIn = 0;
int button = A2;

double mins = 0;
double pMins = 0;
double interval = 1; // Time in minute between readings
double pInter = 0;

void setup() {
  pinMode(button, INPUT);
  
  
  //SD Boot
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1) ;
  }
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    // Wait forever since we cant write data
    while (1) ;
  }
  
  
  // DHT11 Boot
  dht.begin();
  
  // RTC Boot
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
  }
  
  DateTime now = RTC.now();
  pMins = now.minute();  
  
}

void loop() {
//  delay(2000);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Button Reading
  DateTime now = RTC.now();
  
  
  // delay between logs
  if (pMins != now.minute()) {
    pMins = now.minute();
    pInter = mins;
    mins += 1;
  }
  bIn = digitalRead(button);
  
  if (bBounce == false && bIn == 1 && bState == false) {
    bState = true;
    bBounce = true;
  }
  if (bBounce == false && bIn == 1 && bState == true) {
    bState = false;
    bBounce = true;
  }
  if (bBounce == true && bIn == 0) {
    bBounce = false;
  }
  
  
  if (mins >= pInter + interval && bState == true) {
    pInter = mins;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //RTC Reading
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
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //DHT Reading
    double h = dht.readHumidity();
    double t = dht.readTemperature();
    double f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      return;
    }
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Log to SD Card
    String dataString = "";
    
    dataString += (String)h;
    dataString += "|";
    dataString += (String)t;
    dataString += "|";
    dataString += date;
    dataString += "|";
    dataString += time;
    dataFile.println(dataString);
    dataFile.flush();
  }
}
