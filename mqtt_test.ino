#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h> 

// WiFi
const char* ssid = "SKYNET";
const char* wifi_password = "Rubyred3554";

// MQTT
const char* mqtt_server = "192.168.1.28";
const char* mqtt_topic = "soil_temp";
const char* mqtt_username = "darrenrector91";
const char* mqtt_password = "B00ne55428!";
const char* clientID = "soil_sensor_client";

// ds18b20 GPIO pin
const int oneWireBus = 4;

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

// oneWire instance 
OneWire oneWire(oneWireBus);

// Pass oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);
  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
   sensors.begin();
}
void loop() {
  sensors.requestTemperatures();
  float tempF = sensors.getTempFByIndex(0);
  Serial.print(tempF);
  client.publish(mqtt_topic, String(tempF).c_str(), true);
  delay(60000);
}
