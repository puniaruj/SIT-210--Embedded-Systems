#include <Arduino.h>
#include <WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA.h>

#define FIREBASE_HOST "arujhdtask-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDrwCzZC6wZn9PXOxeSoO5z3EEwFstD8TA"
#define WIFI_SSID "Hehe"
#define WIFI_PASSWORD "123456789"

FirebaseData firebaseData;
const String firebasePath = "/status/status";

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize Firebase
  initializeFirebase();

  // Set initial status to "OFF"
  setFirebaseStatus("OFF");
}

void loop()
{
  // Read status from Firebase
  String status = getFirebaseStatus();

  if (status == "RED")
  {
    controlLEDs(HIGH, LOW, LOW);
    Serial.println("RED LED ON");
  }
  else if (status == "YELLOW")
  {
    controlLEDs(LOW, HIGH, LOW);
    Serial.println("YELLOW LED ON");
  }
  else if (status == "GREEN")
  {
    controlLEDs(LOW, LOW, HIGH);
    Serial.println("GREEN LED ON");
  }
  else if (status == "OFF")
  {
    controlLEDs(LOW, LOW, LOW);
    Serial.println("OFF");
  }
  else
  {
    Serial.println("Unknown status: " + status);
  }

  delay(1000); // Adjust the delay as needed
}

void connectToWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initializeFirebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void setFirebaseStatus(const String &status)
{
  if (Firebase.setString(firebaseData, firebasePath, status))
  {
    Serial.println("Firebase status set to: " + status);
  }
  else
  {
    Serial.println("Error setting Firebase status: " + firebaseData.errorReason());
  }
}

String getFirebaseStatus()
{
  if (Firebase.getString(firebaseData, firebasePath))
  {
    return firebaseData.stringData();
  }
  else
  {
    Serial.println("Error getting Firebase status: " + firebaseData.errorReason());
    return "ERROR";
  }
}

void controlLEDs(int red, int yellow, int green)
{
  digitalWrite(2, red);
  digitalWrite(4, yellow);
  digitalWrite(8, green);
}