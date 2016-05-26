#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

File dataFile;

void setup() {
  Serial.begin(9600);


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
}

void loop() {
  String dataString = "";

  dataFile.println(dataString);

  Serial.println(dataString);
  dataFile.flush();
  
  delay(500);
}









