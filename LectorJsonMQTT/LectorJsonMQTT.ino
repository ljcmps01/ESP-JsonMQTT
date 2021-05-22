#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//Numero maximo de dispositivos
#define nDevices 10
//Identificador de dispositivo
#define device_ID 2
//Longitus max del mensaje a enviar
#define MSG_BUFFER_SIZE  (50)

//Configurar estas variables con los datos de tu red WiFi
const char* ssid = "........";
const char* password = "........";

const char* mqtt_server = "broker.mqtt-dashboard.com";
String S_topic= "spain/nodeMCU/" + String(device_ID);
const char* pubTopic = S_topic.c_str();
const char* subTopic = "spain/nodeMCU/";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[MSG_BUFFER_SIZE];
int value = 0;

//Vector donde se guardaran las variables
int Analog[nDevices][2];
//                    id
//Var Analogica 1 [][][][][]
//Var Analogica 2 [][][][][]

bool Bool[nDevices];
//                 id
//Var booleana [][][][][]

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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<256> doc;  
  deserializeJson(doc, payload, length);

  int id = doc["id"];
  Serial.println(id);
  
  int a1 = doc["a1"];
  Serial.println(a1);
  Analog[id][0]=a1;
  
  int a2 = doc["a2"];
  Serial.println(a2);
  Analog[id][1]=a2;
  
  bool b= doc["b"];
  Serial.println(b);
  Bool[id]=b;

  printInfo();
}

void printInfo(){
  //Impresion del contenido de los vectores
  Serial.println("Data obtenida");
  Serial.println("Analogos");
  for(int i=0;i<2;i++){
    for(int j=0;j<10;j++){
      Serial.print(Analog[j][i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("Booleanos: ");
  for(int j=0;j<10;j++){
      Serial.print(Bool[j]);
      Serial.print(" ");
  }
}

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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
