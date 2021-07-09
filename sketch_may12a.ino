#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NodeMCU";  
const char* password = "12345678";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
const int trigP = 2;  //D4 Or GPIO-2 of nodemcu
const int echoP = 0; 
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
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
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
  digitalWrite(trigP, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay 

  digitalWrite(trigP, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low

  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  distance= duration*0.034/2;        //Calculating actual/real distance

  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
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
