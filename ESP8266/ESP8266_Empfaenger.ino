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
const char* state_topic = "Enter_Here_state_topic";

// Befehlstopics
#define rolloUp "### Topic One ###"
#define rolloDown "### Topic Two ###"

// Initalisierung der Variablen
String switch1;
String strTopic;
String strPayload;
#define relayUP D0
#define relayDOWN D1

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.begin(9600);
  delay(100);
 
  // We start by connecting to a WiFi network
  WiFi.hostname(deviceName);
  WiFi.mode(WIFI_STA);
  Serial.println();
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

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  if(strTopic == rolloUp)
    {
    switch1 = String((char*)payload);
    if(switch1 == "ON")
      {
        Serial.println("ON");
        digitalWrite(relayUP, LOW);
      }
    else
      {
        Serial.println("OFF");
        digitalWrite(relayUP, HIGH);
      }
    }
    
    if(strTopic == rolloDown)
    {
    switch1 = String((char*)payload);
    if(switch1 == "ON")
      {
        Serial.println("ON");
        digitalWrite(relayDOWN, LOW);
      }
    else
      {
        Serial.println("OFF");
        digitalWrite(relayDOWN, HIGH);
      }
    }
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceName, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(state_topic);
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
  client.setCallback(callback);

  pinMode(relayUP, OUTPUT);
  digitalWrite(relayUP, HIGH);
  pinMode(relayDOWN, OUTPUT);
  digitalWrite(relayDOWN, HIGH);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
