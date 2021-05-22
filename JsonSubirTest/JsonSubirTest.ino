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

#define device_ID 1
#define MSG_BUFFER_SIZE  (100)

int a1=36,a2=0;
bool tof=1;

const char* ssid = "Campos";
const char* password = "perico15";
const char* mqtt_server = "broker.hivemq.com";
String S_topic= "spain/nodeMCU";
const char* pubTopic = S_topic.c_str();
char msg[MSG_BUFFER_SIZE];

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a la red ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("Conectado a la red WiFi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando conectar con MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.publish(pubTopic, "hello world");
    } else {
      Serial.print("Intento fallido, rc=");
      Serial.print(client.state());
      Serial.println(" re-intentando en 5 segundos");
      delay(5000);
    }
  }
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


void setup() {
  Serial.begin(74880);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  Serial.println(pubTopic);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  CrearJson(device_ID,random(1024),random(1024),false).toCharArray(msg,MSG_BUFFER_SIZE);
   client.publish(pubTopic,msg);
   Serial.println(msg);
   delay(5000);
   CrearJson(random(10),random(1024),random(1024),true).toCharArray(msg,MSG_BUFFER_SIZE);
   client.publish(pubTopic,msg);
   Serial.println(msg);
   delay(5000);
}
