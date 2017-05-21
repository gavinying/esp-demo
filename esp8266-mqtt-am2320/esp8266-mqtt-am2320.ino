/*
 ESP8266 MQTT + AM2320 T&H Sensor example
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Update these with values suitable for your network.
const char* ssid = "......";
const char* password = "......";

// Connect to thingsboard
const char* mqtt_server = "iot.eniwise.com";
const char* mqtt_username = "IqoCKKqy81igp7q6ZpTI";   // Replace with your token
const char* mqtt_password = "";
const char* mqtt_topic = "v1/devices/me/telemetry";
/*
// Connect to iot.eclipse.org
const char* mqtt_server = "iot.eclipse.org";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "your_topic";
*/
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

// DHT
#define DHTPIN 2
#define DHTTYPE DHT22

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect (clientId, mqtt_username, mqtt_password)
    if (client.connect("eniwise-client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  // AM2320 requires at least 2 seconds to obtain accurate data
  if (now - lastMsg > 2000) {
    getAndSendAM2320Data();
    lastMsg = now;
  }
}

void getAndSendAM2320Data()
{
  Serial.println("Collecting AM2320 data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; payload += t; payload += ",";
  payload += "\"humidity\":"; payload += h;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( mqtt_topic, attributes );
  Serial.print("Sent to topic (");
  Serial.print(mqtt_topic);
  Serial.print(") -> " );
  Serial.println( attributes );
}

