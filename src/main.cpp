#include <Arduino.h>

// Sensor Libraries
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Required SD Card libraries
#include <SD.h>
#include <SPI.h>

// Define pin location and sensor type
#define DHTPIN 0
#define DHTTYPE DHT11

#define DS18_PIN 1

#define STATUS_LED 13

float temp, humid, tempDS18;

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

  // Initialize the sensor and Serial baud rate
  Serial.begin(9600);
  dht_sensor.begin();
  DS18_sensor.begin();

  // Initialize the SD Card
  SD.begin(BUILTIN_SDCARD);
  testFile = SD.open("test.txt", FILE_WRITE);

  // Begin writing to file
  if(testFile){
    testFile.println("Hello World!");
    testFile.close();
  }

  testFile = SD.open("test.txt", FILE_READ);
  if(testFile){
    while(testFile.available()){

      Serial.write(testFile.read());
    }
  }

}

void loop() {

  digitalWrite(STATUS_LED, LOW);

  // Get the Temperature and Humidity
  humid = dht_sensor.readHumidity();

  DS18_sensor.requestTemperatures();

  tempDS18 = DS18_sensor.getTempFByIndex(0);

  digitalWrite(STATUS_LED, HIGH);
  delay(2000);

  // Output the data
  Serial.printf("\nHumidity: %.2f\n", humid);
  Serial.printf("Temperature from DS18: %.2f\n", tempDS18);

}