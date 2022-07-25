  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  //Khai báo chân led ở D1
  #define ledPin D1  

  //Khai báo mạng wifi
  const char* ssid = "Duc";
  const char* password = "0906607710";
  //Server mqtt free.
  const char* mqtt_server = "broker.hivemq.com";


  WiFiClient espClient;

  PubSubClient client(espClient);
  unsigned long lastMsg = 0;
  #define MSG_BUFFER_SIZE	(50)
  char msg[MSG_BUFFER_SIZE];
  int value = 0;

void setup_wifi() {
  delay(10);
  // Hàm kết nối wifi
  Serial.println();
  Serial.print("Đang kết nối tới ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  randomSeed(micros());
}
void reconnect() {
  // Hàm kết nối lại.
  while (!client.connected()) {
    Serial.print("Đang kết nối với MQTT....");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("đã kết nối");
      // Once connected, publish an announcement...
      client.publish("cambien", "");
      // ... and resubscribe
      client.subscribe("denled"); //denled sẽ là subcribe để điều khiển đèn
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" kết nối lại sau 5s");
      // Kết nối lại sau 5s
      delay(5000);
    }
  }
}
void ledcontrol(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Lệnh gửi từ server broker [");
  Serial.print(topic);
  int p =(char)payload[0]-'0';
  // If the value is '0' => turn off the led
  if(p==0)
  {
    digitalWrite(ledPin, LOW);
    Serial.println(" Đã tắt đèn!]");
  }
  // If the value is '1' => turn on the led
  if(p==1)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println(" Đã bật đèn!]");
  }
  Serial.println();
} 


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  Serial.println("\nĐịa chỉ IP của máy:");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  
  //Khai báo trạng thái ban đầu của đèn
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

  //Khai báo các chân của esp8266

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
 if (sensorValue2>= 100)
    {
  Serial.print("Trời hiện đang mưa");
  client.publish("cambien", "Trời hiện đang mưa");
  digitalWrite(enable2, HIGH);
  }
  else
  {
  Serial.print("Không phát hiện trời mưa");
  client.publish("cambien", "Không phát hiện trời mưa");
  digitalWrite(enable2, LOW); 
  }
	Serial.println();
	delay(2000);
	//Gọi lại hàm ledcontrol.
	client.setCallback(ledcontrol);
}
