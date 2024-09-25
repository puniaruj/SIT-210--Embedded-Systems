#include <WiFiNINA.h>
#include <FirebaseESP32.h>  // Firebase library for ESP32/ESP8266/Arduino

// Replace with your network credentials
const char* ssid = "Hehe";
const char* password = "123456789";

// Firebase setup
#define FIREBASE_HOST "https://embedded-hd-task-default-rtdb.asia-southeast1.firebasedatabase.app/"  // e.g., "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "E1YNyiridhEI4y0SLf3SZ6znLgZZSKpMCHpu0EZw"

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

const int redLED = 2;
const int greenLED = 3;
const int blueLED = 4;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Initialize Firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.getBool(firebaseData, "/trafficLights/red")) {
    digitalWrite(redLED, HIGH);
  } else {
    digitalWrite(redLED, LOW);
  }

  if (Firebase.getBool(firebaseData, "/trafficLights/green")) {
    digitalWrite(greenLED, HIGH);
  } else {
    digitalWrite(greenLED, LOW);
  }

  if (Firebase.getBool(firebaseData, "/trafficLights/blue")) {
    digitalWrite(blueLED, HIGH);
  } else {
    digitalWrite(blueLED, LOW);
  }

  delay(1000);  // Poll Firebase every second
}
