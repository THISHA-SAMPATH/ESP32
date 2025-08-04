#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#include <time.h>

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Airtel_jaga_6001";
const char* password = "air36300";

const char* host = "itesopcbbtzqvjwnolzf.supabase.co";
const char* endpoint = "/rest/v1/sensor_data";
const int httpsPort = 443;
const char* bearer_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Iml0ZXNvcGNiYnR6cXZqd25vbHpmIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NTM4ODk1NjgsImV4cCI6MjA2OTQ2NTU2OH0.sroe9ltWKG4fqi_vao18lQrmkm9YvoGYjU0swRApcV4";  // Replace with your actual token

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);

  // Start WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Start DHT sensor
  dht.begin();

  // Set up NTP time (IST)
  configTime(19800, 0, "pool.ntp.org");
  Serial.println("Waiting for NTP time sync...");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nTime synchronized!");

  client.setInsecure(); // okay for testing
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Get current time
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo); // ✅ gives IST (because of configTime offset)


  char timeString[30];
  strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  String timestamp = String(timeString);

  // Create payload with timestamp
  String payload = "{\"Temperature\": " + String(temp) + 
                   ", \"Humidity\": " + String(hum) + 
                   ", \"Timestamp\": \"" + timestamp + "\"}";

  if (client.connect(host, httpsPort)) {
    Serial.println("Connected to Supabase!");

    client.println("POST " + String(endpoint) + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.println("apikey: " + String(bearer_token));
    client.println("Authorization: Bearer " + String(bearer_token));
    client.println("Prefer: return=representation");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);

    Serial.println("Sent data:");
    Serial.println(payload);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
    }

    String response = client.readString();
    Serial.println("Response:");
    Serial.println(response);

    client.stop();
  } else {
    Serial.println("Connection to Supabase failed.");
  }

  delay(10000); // Wait 10 sec
}
