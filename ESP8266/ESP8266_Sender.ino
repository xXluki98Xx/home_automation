#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WLan SSID und Passwort
#define wifi_ssid "Enter_Here_SSID"
#define wifi_password "Enter_Here_Password"

// MQTT
#define mqtt_server "Enter_Here_Server_IP"
#define mqtt_user "Enter_Here_Username"
#define mqtt_password "Enter_Here_Password"

// MQTT Topic's
const char* door_topic = "Enter_Here_Topic";

// Gerätenamen, gilt für WLAN und MQTT
const char* deviceName = "Enter_Here_Name";

long lastMsg = 0;

#define door D0

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.begin(9600);
  delay(10);

  WiFi.hostname(deviceName);
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);
  
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
    // Attempt to connect
    if (client.connect(deviceName, mqtt_user, mqtt_password)) {
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
 
void setup()
{
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);

  pinMode(door, INPUT);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

// Enter Here Code
}
