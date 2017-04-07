/*
 NodeMCU Web Server Example

 This code was tested with Lolin NodeMCU v3

 Thiago Ferauche - ferauche.blogspot.com
 01/04/2017
*/

#include <ESP8266WiFi.h>

//Variáveis para conexão WiFi
const char* ssid = "robo_ssid";
const char* password = "";
 
WiFiServer server(80);

//Definir portas a serem usadas
byte red_led = D0; //D0;
byte green_led = D3; //D3;
byte button = D5; // D5

//Definir variaveis de controle
byte status_button = -1;
int intensidade = 1000;

// the setup function runs once when you press reset or power the board
void setup() {
  //setamdp mode de operacao das portas
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(button, INPUT);

  //configurando velocidade da porta serial
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //pisca green_led
    if(digitalRead(green_led)==HIGH) {
      digitalWrite(green_led, LOW);
    }
    else {
      digitalWrite(green_led, HIGH);
    }
  }

  //acende green_led esta conectado
  digitalWrite(green_led, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  printConnectionConfig();

}

// the loop function runs over and over again forever
void loop() {
   if(digitalRead(button)!=status_button) {
    Serial.print("Chave...: ");
    Serial.println(digitalRead(button));
    status_button = digitalRead(button);
  }

  if(status_button==1) {
    piscar(intensidade);
  }
  
  WiFiClient client = server.available();
  if(!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  buf += "<html>\r\n";
  buf += "<title>NodeMCU ESP8266 Web Server</title>\n";
  buf += "<body>\n";
  buf += "<h3>NodeMCU ESP8266 Web Server</h3>\n";
  buf += "<p>Intesidade <a href=\"?function=more\"><button>+</button></a> <a href=\"?function=less\"><button>-</button></a></p>";
  buf += "</html>\n";
 
  client.print(buf);
  client.flush();

  if(req.indexOf("more") != -1) {
    intensidade-=100;
  }
  else if(req.indexOf("less") != -1) {
    intensidade+=100;
  }
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("client disconnected");
}

void piscar(int intensidade) {
  digitalWrite(red_led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(intensidade);                       
  digitalWrite(red_led, LOW);    // turn the LED off by making the voltage LOW
  delay(intensidade);                 
}

void printConnectionConfig() {
  //Definir variavel para endereco MAC
  byte mac[6];

  Serial.println(WiFi.localIP());
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

