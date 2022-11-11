#include "model.h"
#include <WiFi.h>

Eloquent::ML::Port::RandomForest classifier;
const char* ssid = "";// Your Wifi access id
const char* password = ""; // Your wifi p[assword
WiFiServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>

  Check 1 2
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    Rohit Check
  </p>
  <p>
    Wow wow 
  </p>
</body>
</html>)rawliteral";

void classify() {// Takes input from sensor data and then return the approplriate hygiene index
    float x_sample[] = { /* fill this vector with sample values */ };

    Serial.print("Predicted class: ");
    Serial.println(classifier.predictLabel(x_sample));
}

void setup() {
  // put your setup code here, to run once:
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature and humidity
            client.println(index_html);     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   
