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
int led = A3;
int button = A2;

double mins = 0;
double pMins = 0;
double interval = 1; // Time in minute between readings
double pInter = 0;

void setup() {
  Serial.begin(9600);
  
  Serial.println("intilizing local variables");
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  
  
  //SD Boot
  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  Serial.println("File opened");
  
  
  // DHT11 Boot
  Serial.println("DHT11 Starting");
  dht.begin();
  Serial.println("DHT11 booted");
  
  // RTC Boot
  Wire.begin();
  RTC.begin();
  Serial.println("rtc starting");
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    // RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  DateTime now = RTC.now();
  pMins = now.minute();
  Serial.println("rtc booted");
  
  
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
    digitalWrite(led, LOW);
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
      Serial.println("Failed to read from DHT sensor!");
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
    Serial.println(dataString);
    dataFile.flush();
    
  }
  if (bState == true) {
    digitalWrite(led, HIGH);
  }
  if (bState == false) {
    digitalWrite(led, LOW);
  }
}
