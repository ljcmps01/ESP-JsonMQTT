// Este programa contiene la funcion de callback que será llamada
// Cada vez que la node MCU reciba un mensaje a travez de su suscripción

#include <ArduinoJson.h>

#define nDevices 10
int Analog[nDevices][2];
bool Bool[nDevices];

void setup() {
  Serial.begin(115200);
  Serial.println();
//vector char que simularia el mensaje recibido de mqtt
  char input []=
      "{\"id\":1,\"a1\":1030,\"a2\":48,\"b\":true}";
      
//  Creo el JSON y tiro la informacion del input a el
  StaticJsonDocument<256> doc;  
  deserializeJson(doc, input);

//  inicio el guardado de información
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
//fin del guardado de informacion

//Impresion del contenido de los vectores
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

void loop() {
}
