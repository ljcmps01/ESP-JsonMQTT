#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "ljcmps"
#define AIO_KEY         "ab2efbfbaaab49c18dfe51ce389d8e12"

//Numero maximo de dispositivos
#define nDevices 10
//Identificador de dispositivo
#define device_ID 0
//Longitus max del mensaje a enviar
#define MSG_BUFFER_SIZE  (50)
//Tiempo de espera entre cada envio de datos
#define TIEMPO 2000

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test");

// Setup a feed called 'onoff' for subscribing to changes.


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).

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

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  setup_wifi();
  Serial.println(F("Adafruit MQTT demo"));

  // Setup MQTT subscription for onoff feed.
 
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! test.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
  delay(5000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
