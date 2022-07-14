
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ledPin D1  //LED at GPIO4 D1
// Update these with values suitable for your network.

const char* ssid = "Duc";
const char* password = "0906607710";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;

PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length)

{

  Serial.print("Command from MQTT broker is : [");

  Serial.print(topic);

  int p =(char)payload[0]-'0';

  // if MQTT comes a 0 turn off LED on D1

  if(p==0)

  {

    digitalWrite(ledPin, LOW);

    Serial.println(" Turn Off ledPin! ");

  }

  // if MQTT comes a 1, turn on ledPin on pin D2

  if(p==1)

  {

    digitalWrite(ledPin, HIGH);

    Serial.println(" Turn On ledPin! ");

  }

  Serial.println();

} //end callback


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("cambien", "");
      // ... and resubscribe
      client.subscribe("cambien1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

  int sensorPin = A0;
  int enable2 = 13;
  int sensorValue2 = 0;
	unsigned char status_led=0;


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
sensorValue2 = analogRead(sensorPin);
sensorValue2 = constrain(sensorValue2, 150, 440); 
sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
if (sensorValue2>= 20)
{
  Serial.print("Troi hien dang mua");
  client.publish("cambien", "Troi hien dang mua");
  
  
  digitalWrite(enable2, HIGH);
  }
  else
  
{
  Serial.print("Khong phat hien troi mua");
  client.publish("cambien", "Khong phat hien troi mua");

  
  digitalWrite(enable2, LOW); 
  }
//Serial.print("Rain value:       ");
//Serial.println(sensorValue2);
Serial.println();
delay(1500);
client.setCallback(callback);
}
