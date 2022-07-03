#include <Arduino.h>
#include <DHT.h>

// Define pin location and sensor type
#define DHTPIN 0
#define DHTTYPE DHT11

float temp, humid;

// Instantiate the sensor object
DHT dht_sensor(DHTPIN, DHTTYPE);

void setup() {

  // Initialize the sensor and Serial baud rate
  Serial.begin(9600);
  dht_sensor.begin();
}

void loop() {

  // Wait 1 second before next read
  delay(1000);

  // Get the Temperature and Humidity
  temp = dht_sensor.readTemperature(true);
  humid = dht_sensor.readHumidity();

  // Output the data
  Serial.printf("\nHumidity: %.2f\n", humid);
  Serial.printf("Temperature: %.2f\n", temp);

}