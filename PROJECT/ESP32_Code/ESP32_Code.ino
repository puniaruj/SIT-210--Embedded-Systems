#include <Wire.h>

#define I2C_ADDRESS 8  // I2C address of the Arduino Nano 33 IoT

float humidity = 0;          // Humidity (DHT22)
float roomTemp = 0;          // Room temperature (DHT22)
int mq2Value = 0;            // Gas sensor value (MQ2)

void setup() {
  Wire.begin(21, 22);        // Set up I2C pins for ESP32 (SDA = GPIO 21, SCL = GPIO 22)
  Serial.begin(9600);        // Initialize serial communication for debugging
}

void loop() {
  // Request data from the Arduino Nano 33 IoT
  Wire.requestFrom(I2C_ADDRESS, 10);  // Request 10 bytes (2 floats = 8 bytes + 2 bytes for gas sensor value)
  
  // Receive and process the float values (humidity and room temperature)
  humidity = receiveFloatFromI2C();  // Receive the humidity value
  roomTemp = receiveFloatFromI2C();  // Receive the room temperature value
  
  // Receive the integer value (gas sensor)
  mq2Value = Wire.read();            // Read gas sensor value
  
  // Debugging: Print received sensor values to Serial Monitor
  Serial.print("Humidity (DHT22): "); Serial.println(humidity);
  Serial.print("Room Temp (DHT22): "); Serial.println(roomTemp);
  Serial.print("Gas Sensor Value: "); Serial.println(mq2Value);

  delay(2000);  // Wait before next reading
}

// Function to receive a float value over I2C
float receiveFloatFromI2C() {
  union {
    float f;
    uint8_t b[4];
  } data;
  
  for (int i = 0; i < 4; i++) {
    data.b[i] = Wire.read();  // Receive 4 bytes of data
  }

  return data.f;  // Convert and return the float value
}
