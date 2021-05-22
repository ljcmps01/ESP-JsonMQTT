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

int id=98,a1=36,a2=0;
bool tof=1;

String CrearJson();

void setup() {
  Serial.begin(74880);
 Serial.println(CrearJson(124,54325,7654,false));
 Serial.println(CrearJson(124,545,7214,0));
 Serial.println(CrearJson(124,5435,7586,1));
 Serial.println(CrearJson(124,64325,2564,true));
}

void loop() {

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
