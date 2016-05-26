#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

float temp = 0; // Celcius
float hum = 0; // humidity

void setup() {
  Serial.begin(9600);
  
  Serial.println("DHT11 Starting");
  dht.begin();
  Serial.println("DHT11 booted");
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
}
