#include "DHT.h"
#include "WiFiNINA.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include ThingSpeak header file after other header files and custom macros

// DHT sensor configuration
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// WiFi and ThingSpeak configuration
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
WiFiClient client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);      // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  dht.begin();  // Initialize the DHT sensor
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Read humidity and temperature from the DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius
  float f = dht.readTemperature(true);  // Fahrenheit

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit and Celsius
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Print sensor values to Serial Monitor
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  // Set ThingSpeak fields with the sensor values
  ThingSpeak.setField(1, t);  // Temperature in Celsius
  ThingSpeak.setField(2, f);  // Temperature in Fahrenheit
  ThingSpeak.setField(3, h);  // Humidity
  ThingSpeak.setField(4, hif);  // Heat index in Fahrenheit

  // Determine status message based on the temperature
  String myStatus;
  if (t > 30) {
    myStatus = String("It's hot!");
  } else if (t < 15) {
    myStatus = String("It's cold!");
  } else {
    myStatus = String("Temperature is moderate.");
  }

  // Set the status
  ThingSpeak.setStatus(myStatus);

  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(1000); // Wait 20 seconds to update the channel again
}