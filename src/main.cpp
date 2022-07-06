#include <Arduino.h>

// Sensor Libraries
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Required SD Card libraries
#include <SD.h>
#include <SPI.h>

// Required Time library
#include <TimeLib.h>

// Define pin location and sensor type
#define DHTPIN 0
#define DHTTYPE DHT11

#define DS18_PIN 1
#define STATUS_LED 13

float temp, humid, tempDS18;
int current_min;

// Instantiate the sensor object
DHT dht_sensor(DHTPIN, DHTTYPE);
OneWire onwire(DS18_PIN);
DallasTemperature DS18_sensor(&onwire);

// Initialize FILE object
File testFile;

void setup() {

  // LED status initialization
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  setTime(0,0,0,6,7,2022);
  current_min = minute();

  // Initialize the sensor and Serial baud rate
  Serial.begin(9600);
  dht_sensor.begin();
  DS18_sensor.begin();

  // Initialize the SD Card
  SD.begin(BUILTIN_SDCARD);

  // Removes data automatically stored in the SD card
  if(SD.exists("data.txt")){
    SD.remove("data.txt");
  }

  testFile = SD.open("data.txt", FILE_WRITE_BEGIN);

  // Begin writing to file
  if(testFile){
    Serial.println("Setting up the file. Printing Headers");
    testFile.println("Date, Time, Farenheit_Temperature, Humidity_Percentage");
    testFile.close();
  }
}

void loop() {
  digitalWrite(STATUS_LED, LOW);

  // Get the Temperature and Humidity
  humid = dht_sensor.readHumidity();
  DS18_sensor.requestTemperatures();
  tempDS18 = DS18_sensor.getTempFByIndex(0);

  digitalWrite(STATUS_LED, HIGH);
  delay(1000);

  // Output the data every minute
  if(current_min != minute()){;
    current_min = minute();
    testFile = SD.open("data.txt", FILE_WRITE);
    if(testFile){
      testFile.printf("%d/%d/%d, %d:%d:%d, %.2f, %.2f\n", month(), day(), year(), hour(), minute(), second(), tempDS18, humid);
      testFile.close();
    }
  }
}