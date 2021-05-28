/* Nombres de variables utilizados: nDevices, device_ID, MSG_BUFFER_SIZE, TIEMPO, ssid, password, mqtt_server, pubTopic, subTopic, msg, 
 *                                  value, Analog[][], Bool[], setup_wifi, callback, topic, payload, i, doc[], id, a1, a2, 
 *                                  b, printInfo, j, clientId, CrearJson, x, b1, b2, d, Json.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//Numero maximo de dispositivos
#define nDevices 10

//Identificador de dispositivo (cambiar para cada NodeMCU)
#define device_ID 2

//Longitud max del mensaje a enviar
#define MSG_BUFFER_SIZE  (50)

//Tiempo de espera (en milisegundos) entre cada envio de datos
#define TIEMPO 2000

//Configurar estas variables con los datos de tu red WiFi
const char* ssid = "...";
const char* password = "...";

//datos del broker mqtt
const char* mqtt_server = "broker.hivemq.com";
const char* pubTopic = "spain/nodeMCU";
const char* subTopic = "spain/nodeMCU";

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

//Funciones conexion
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

//Funciones de informacion
void printInfo(); 
String CrearJson(int id,int a1,int a2,bool b);
//**IMPORTANTE**: para mandar la informacion hay que modificar los parametros que se pasan en el loop
//Las variables que hay ahora en el loop son a modo de prueba e imprimer numeros aleatorios
//int id se modifica desde el #define device_ID
//int a1 y a2 son las variables analogicas 1 y 2 respectivamente
//bool b es la variable booleana



void setup() {
  Serial.begin(115200);
  setup_wifi();                         //Me conecto a WiFi
  client.setServer(mqtt_server, 1883);  //Configuro el server MQTT
  client.setCallback(callback);         //Configuro la funcion a la que llamar cuando llega info
}

void loop() {

  if (!client.connected()) {            //Chequeo que el dispositivo este conectado a MQTT
    reconnect();
  }
  client.loop();

  //Preparo la informacion para poder enviarla
  //Modificar los parametros random(1024), ambos, y random(10)>5?true:false con los datos analogicos y booleano respectivamente
  CrearJson(device_ID,random(1024),random(1024),random(10)>5?true:false).toCharArray(msg,MSG_BUFFER_SIZE);
   client.publish(pubTopic,msg);        //Publico el mensaje
   Serial.print("");
   delay(TIEMPO);
}


//////////////////////////////////////////
//***FUNCIONES CONFIGURACION INTERNET***//
//////////////////////////////////////////

//Conexion a red WiFi
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

//Respuesta del programa cuando recibe informacion de mqtt
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recibido [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<256> doc;  
  deserializeJson(doc, payload, length);

  int id = doc["id"];
  Serial.println("Data obtenida del id #"+String(id));
  
  int a1 = doc["a1"];
  Serial.println("Analogica 1: "+String(a1));
  Analog[id][0]=a1;
  
  int a2 = doc["a2"];
  Serial.println("Analogica 2: "+String(a2));
  Analog[id][1]=a2;
  
  bool b= doc["bool"];
  Serial.println("Booleana: "+String(b)+"\n");
  Bool[id]=b;

  printInfo();
}

//Función de conexion y reconexion a MQTT + suscripción
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


//////////////////////////////////////////
//*******FUNCIONES DE INFORMACION*******//
//////////////////////////////////////////

//Imprime los vectores de información 
void printInfo(){
  //Impresion del contenido de los vectores
  Serial.println("Vector de información de las NodeMCU");
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

//Funcion que prepara la información para enviarla a MQTT
String CrearJson(int id,int a1,int a2,bool b){
  String Json = "{\"id\":";
 Json += String(id);
 Json += ", \"a1\":";
 Json += String(a1);
 Json += ", \"a2\":";
 Json += String(a2);
 Json += ", \"bool\":";
 Json += String(b);
 Json += "}";
 return Json;
}
