#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

// Pin definitions
#define DHTPIN 2
#define DHTTYPE DHT22
#define MQ2_PIN A2
#define PULSE_PIN A0
#define HAND_DETECTION_THRESHOLD 350

// I2C LCD address
#define LCD_ADDR 0x27 // Check this with an I2C scanner
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// WiFi and MQTT server details
const char* ssid = "Hehe";             // Replace with your WiFi SSID
const char* password = "123456789";     // Replace with your WiFi password
const char* mqtt_server = "broker.emqx.io";      // Replace with your MQTT broker IP
const int mqtt_port = 1883;

// Sensor objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

// WiFi and MQTT client
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Timing variables for non-blocking delays
unsigned long lastPublishTime = 0;
unsigned long lastDisplayTime = 0;
const long publishInterval = 2000;  // Publish every 2 seconds
const long displayInterval = 2000;  // Update LCD every 2 seconds

void setup() {
  Serial.begin(9600);

  dht.begin();
  mlx.begin();
  pinMode(MQ2_PIN, INPUT);

  // Initialize LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Connect to WiFi
  connectToWiFi();

  // Set up MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Non-blocking MQTT reconnect
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Current time for managing intervals
  unsigned long currentTime = millis();

  // Read sensors and update display every displayInterval
  if (currentTime - lastDisplayTime >= displayInterval) {
    updateDisplay();
    lastDisplayTime = currentTime;
  }

  // Publish data to MQTT broker every publishInterval
  if (currentTime - lastPublishTime >= publishInterval) {
    publishData();
    lastPublishTime = currentTime;
  }
}

// Function to connect to WiFi
void connectToWiFi() {
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

// Function to connect to MQTT broker
void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(2000);
    }
  }
}

// Function to publish data to MQTT and print to Serial Monitor
void publishData() {
  float roomTemp = dht.readTemperature();
  float roomHumidity = dht.readHumidity();
  int mq2Value = analogRead(MQ2_PIN) - 250;

  int pulseRawValue = analogRead(PULSE_PIN);

  int pulseBPM = 0;
  String handDetected = "No";
  if (pulseRawValue > HAND_DETECTION_THRESHOLD) {
    pulseBPM = map(pulseRawValue, 355, 500, 80, 120);
    handDetected = "Yes";
  }

  String payload = "{";
  payload += "\"roomTemp\":" + String(roomTemp) + ",";
  payload += "\"roomHumidity\":" + String(roomHumidity) + ",";
  payload += "\"airQuality\":" + String(mq2Value) + ",";
  payload += "\"pulseBPM\":" + String(pulseBPM) + ",";
  payload += "\"handDetected\":\"" + handDetected + "\"";
  payload += "}";

  client.publish("sensor/data", payload.c_str());

  // Print data to Serial Monitor
  Serial.println("Publishing Data:");
  Serial.println(payload);
}

// Function to update LCD display and print readings to Serial Monitor
void updateDisplay() {
  float roomTemp = dht.readTemperature();
  float roomHumidity = dht.readHumidity();
  int mq2Value = analogRead(MQ2_PIN) - 250;
  int pulseRawValue = analogRead(PULSE_PIN);
  int pulseBPM = 0;
  String handDetected = "No";

  if (pulseRawValue > HAND_DETECTION_THRESHOLD) {
    pulseBPM = map(pulseRawValue, 355, 500, 80, 120);
    handDetected = "Yes";
  }

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(roomTemp);
  lcd.print("C");
  delay(500);

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(roomHumidity);
  lcd.print("%");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality: ");
  lcd.print(mq2Value);
  delay(500);

  lcd.clear();
  if (pulseRawValue > HAND_DETECTION_THRESHOLD) {
    lcd.setCursor(0, 0);
    lcd.print("Pulse: ");
    lcd.print(pulseBPM);
    lcd.print(" BPM");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("No hand detected");
  }
  delay(500);

  // Print data to Serial Monitor
  Serial.println("Displaying Data:");
  Serial.print("Room Temperature: ");
  Serial.print(roomTemp);
  Serial.println(" C");
  
  Serial.print("Room Humidity: ");
  Serial.print(roomHumidity);
  Serial.println(" %");

  Serial.print("Air Quality (MQ2): ");
  Serial.println(mq2Value);

  if (pulseRawValue > HAND_DETECTION_THRESHOLD) {
    Serial.print("Pulse: ");
    Serial.print(pulseBPM);
    Serial.println(" BPM");
  } else {
    Serial.println("No hand detected");
  }
}
