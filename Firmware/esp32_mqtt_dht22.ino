// -------------------- LIBRARIES --------------------
// WiFi.h        -> ESP32 WiFi connectivity
// PubSubClient  -> MQTT client library
// DHTesp.h     -> DHT sensor library optimized for ESP32
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// -------------------- HARDWARE CONFIG --------------------
// GPIO pin where DHT22 DATA line is connected
// (In simulation, this maps to D2 / GPIO 9)
const int DHT_PIN = 9;

// Create DHT sensor object
DHTesp dhtSensor;

// -------------------- WIFI CONFIG --------------------
// Wokwi provides an open WiFi network
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Public MQTT broker (no authentication)
const char* mqtt_server = "broker.hivemq.com";

// -------------------- MQTT OBJECTS --------------------
// WiFi client required by MQTT
WiFiClient espClient;

// MQTT client instance
PubSubClient client(espClient);

// -------------------- TIMING VARIABLES --------------------
// Used to control how often sensor data is published
unsigned long lastMsg = 0;

// Not used directly, but kept from template
#define MSG_BUFFER_SIZE  (50)

// Variables to store sensor values
float temp = 0;
float hum = 0;

// Unused variable (from template)
int value = 0;

// -------------------- WIFI SETUP FUNCTION --------------------
// Connects ESP32 to WiFi network
void setup_wifi() {

  delay(10);   // Small startup delay

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Set ESP32 to station mode (connect to router)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Block until WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Seed random generator (used for MQTT client ID)
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// -------------------- MQTT CALLBACK --------------------
// This function is called whenever a message
// arrives on a subscribed MQTT topic
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Print received payload character-by-character
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Simple LED control logic:
  // If first character of payload is '1' -> LED ON
  // Else -> LED OFF
  if ((char)payload[0] == '1') {
    digitalWrite(2, LOW);   // Active LOW LED -> ON
  } else {
    digitalWrite(2, HIGH);  // LED OFF
  }
}

// -------------------- MQTT RECONNECT --------------------
// Ensures ESP32 stays connected to MQTT broker
void reconnect() {

  // Loop until MQTT connection is established
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt MQTT connection
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");

      // Publish a startup message
      client.publish("ESP/mqtt", "ESP");

      // Subscribe to control topic
      client.subscribe("ESP/mqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait before retrying
      delay(5000);
    }
  }
}

// -------------------- SETUP FUNCTION --------------------
// Runs once at boot
void setup() {

  // Configure built-in LED pin
  pinMode(2, OUTPUT);

  // Start serial communication
  Serial.begin(115200);

  // Connect to WiFi
  setup_wifi();

  // Configure MQTT broker and callback
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Initialize DHT22 sensor
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

// -------------------- MAIN LOOP --------------------
// Runs repeatedly after setup()
void loop() {

  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    reconnect();
  }

  // Handle incoming/outgoing MQTT messages
  client.loop();

  // Publish sensor data every 2 seconds
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    // Read temperature and humidity from DHT22
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    // Convert temperature to string
    String temp = String(data.temperature, 2);
    Serial.print("Temperature: ");
    Serial.println(temp);

    // Publish temperature to MQTT
    client.publish("ESP/DHT22/temperature", temp.c_str());

    // Convert humidity to string
    String hum = String(data.humidity, 1);
    Serial.print("Humidity: ");
    Serial.println(hum);

    // Publish humidity to MQTT
    client.publish("ESP/DHT22/humidity", hum.c_str());
  }
}
