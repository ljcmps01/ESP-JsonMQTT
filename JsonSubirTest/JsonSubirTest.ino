/*OBJETIVO: imprimir una string de la forma: "{ \"id\":..., \"a1\":..., \"a2\":..., \"bool\":...}"
 * 
 * IDEA DE FUNCIONAMIENTO:
 * inicio string "json" con "{\"id\":";
 * agarro la variable id y la transformo en string
 * La sumo al final de la string json
 * agrego el atributo de la siguiente variable a la string json (", \"a1\":")
 * Agarro la siguiente variable (a1) la transformo en string
 * La sumo al final de la string json
 * Repetir con todas las variables
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


int id=98,a1=36,a2=0;
bool tof=1;

const char* ssid = "Campos";
const char* password = "perico15";
const char* mqtt_server = "retropie.local";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

String CrearJson();

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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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

void setup() {
  Serial.begin(74880);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  CrearJson(124,54325,7654,false).toCharArray(msg,100);
   client.publish("outopic",msg);
   delay(5000);
   CrearJson(124,525,54,true).toCharArray(msg,100);
   client.publish("outopic",msg);
   delay(5000);
}

String CrearJson(int i,int b1,int b2,bool d){
  String Json = "{\"id\":";
 Json += String(i);
 Json += ", \"a1\":";
 Json += String(b1);
 Json += ", \"a2\":";
 Json += String(b2);
 Json += ", \"bool\":";
 Json += String(d);
 Json += "}";
 return Json;
}
