// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.
//
// https://arduinojson.org/v6/example/string/

#include <ArduinoJson.h>

#define nDevices 10
int Analog[nDevices][2];
bool Bool[nDevices];

void setup() {
  StaticJsonDocument<256> doc;  
  
  Serial.begin(115200);
  Serial.println();

  // You can use a String as your JSON input.
  // WARNING: the string in the input  will be duplicated in the JsonDocument.
  char input []=
      "{\"id\":1,\"a1\":1030,\"a2\":48,\"b\":true}";
  deserializeJson(doc, input);
  
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

//  int t=Analog[id][0]+Analog[id][1];
//  Serial.println(t);
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
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. On several occasions, it
// shows how you can avoid String in your program.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
