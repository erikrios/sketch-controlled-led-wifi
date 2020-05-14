#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED_PIN 13
#define SSID "Double_Encryption"
#define PASSWORD "ERIKRIOSETIAWAN"

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // start the server
  server.begin();
  Serial.println("Server started");

  // print the ip address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // put your main code here, to run repeatedly:
  // check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }
  
  // read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)
  {
    digitalWrite(LED_PIN, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(LED_PIN, LOW);
    value = LOW;
  }
  
  // return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<HTML>");

  client.print("Led is now: ");

  if (value == HIGH)
  {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br/>");
  client.println("</HTML>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}