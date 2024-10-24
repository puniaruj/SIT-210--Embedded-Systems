#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2             // Pin for DHT sensor
#define DHTTYPE DHT22        // DHT22 sensor type
#define MQ2_PIN A1           // Pin for MQ2 gas sensor
#define I2C_ADDRESS 8        // I2C address to communicate with ESP32

DHT dht(DHTPIN, DHTTYPE);

// Variables for sensor readings
float humidity = 0;          // Humidity (DHT22)
float roomTemp = 0;          // Room temperature (DHT22)
int mq2Value = 0;            // Gas sensor value

void setup() {
  Wire.begin();              // Initialize I2C communication
  dht.begin();               // Initialize DHT sensor

  Serial.begin(9600);        // For debugging
}

void loop() {
  // Read sensor data
  humidity = dht.readHumidity();
  roomTemp = dht.readTemperature();
  mq2Value = analogRead(MQ2_PIN);  // Read gas sensor

  // Debugging: Print sensor readings to Serial Monitor
  Serial.print("Humidity (DHT22): "); Serial.println(humidity);
  Serial.print("Room Temp (DHT22): "); Serial.println(roomTemp);
  Serial.print("Gas Sensor Value: "); Serial.println(mq2Value);

  // Send sensor data over I2C to ESP32
  Wire.beginTransmission(I2C_ADDRESS);
  sendFloatOverI2C(humidity);       // Send humidity
  sendFloatOverI2C(roomTemp);       // Send room temperature
  Wire.write(mq2Value);             // Send gas sensor value (integer)
  Wire.endTransmission();

  delay(2000); // Wait before sending again
}

// Function to send float over I2C
void sendFloatOverI2C(float value) {
  uint8_t *bytePtr = (uint8_t*)&value;
  for (int i = 0; i < 4; i++) {
    Wire.write(bytePtr[i]);
  }
}
