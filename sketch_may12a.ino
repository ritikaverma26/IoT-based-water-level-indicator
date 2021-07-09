#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NodeMCU";  
const char* password = "12345678";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
const int trigP = 2;  //D4
const int echoP = 0; //D3
long duration;
int distance=0;
String stringdistance=String(distance);

void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/measure", measure_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  pinMode(trigP, OUTPUT);  
  pinMode(echoP, INPUT);

}

void loop() {
  server.handleClient();
}
void handle_OnConnect() {
  Serial.println("Status is connected");
  server.send(200, "text/html", SendHTML(stringdistance)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
void measure_OnConnect(){
  digitalWrite(trigP, LOW);   
  delayMicroseconds(2);       

  digitalWrite(trigP, HIGH);  
  delayMicroseconds(10);      
  digitalWrite(trigP, LOW);   

  duration = pulseIn(echoP, HIGH);   
  distance= duration*0.034/2;        

  Serial.print("Distance = ");        
  Serial.println(distance);
  stringdistance=String(distance);
  server.send(200, "text/html", SendHTML(stringdistance));
  Serial.println(stringdistance);
}
String SendHTML(String stringdistance){
  String ptr="<!DOCTYPE html><html><head><script type=\"text/javascript\">function operator(l){document.getElementById(\"level\").innerHTML=l;}</script><title>Water level indicator</title></head><body onload=\"operator(";
  ptr +=stringdistance;
  ptr +=")\"><p>The water level is</p><br><h3 id=\"level\"></h3><button><a href=\"/measure\">Refresh</a></button></body></html>";
  return ptr;
  }
