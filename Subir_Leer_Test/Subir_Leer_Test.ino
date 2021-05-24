/* Nombres de variables utilizados: nDevices, device_ID, MSG_BUFFER_SIZE, TIEMPO, ssid, password, mqtt_server, pubTopic, subTopic, msg, 
 *                                  value, A1, A2, tof, Analog[][], Bool[][], setup_wifi, callback, topic, payload, i, doc[], id, a1, a2, 
 *                                  b, printInfo, j, clientId, CrearJson, x, b1, b2, d, Json.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//Numero maximo de dispositivos
#define nDevices 10
//Identificador de dispositivo
#define device_ID 0
//Longitus max del mensaje a enviar
#define MSG_BUFFER_SIZE  (50)
//Tiempo de espera entre cada envio de datos
#define TIEMPO 2000

//Configurar estas variables con los datos de tu red WiFi
const char* ssid = "...";
const char* password = "...";

const char* mqtt_server = "broker.hivemq.com";
const char* pubTopic = "spain/nodeMCU";
const char* subTopic = "spain/nodeMCU";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[MSG_BUFFER_SIZE];
int value = 0;

int A1=36,A2=0;
bool tof=1;

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
  Serial.println("id #"+String(id));
  
  int a1 = doc["a1"];
  Serial.println("Analogica 1: "+String(a1));
  Analog[id][0]=a1;
  
  int a2 = doc["a2"];
  Serial.println("Analogica 2: "+String(a2));
  Analog[id][1]=a2;
  
  bool b= doc["bool"];
  Serial.println("Booleana: "+String(b)+"\n");
  Bool[id]=b;

  printInfo(id);
}

void printInfo(int id){
  //Impresion del contenido de los vectores
  Serial.print("Data obtenida de la NodeMCU #");
  Serial.println(id);
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
  Serial.println("\n\n");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Intentando conectar con MQTT...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.publish(pubTopic, "hello world");
      // ... and resubscribe
      client.subscribe(subTopic);
    } else {
      Serial.print("Intento fallido, rc=");
      Serial.print(client.state());
      Serial.println(" re-intentando en 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

String CrearJson(int x,int b1,int b2,bool d){
  String Json = "{\"id\":";
 Json += String(x);
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

  CrearJson(device_ID,random(1024),random(1024),random(10)>5?true:false).toCharArray(msg,MSG_BUFFER_SIZE);
   client.publish(pubTopic,msg);
   Serial.print("");
   delay(TIEMPO);
}
