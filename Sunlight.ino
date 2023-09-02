#include "arduino_secrets.h"
#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

WiFiClient client1;
WiFiClient client2;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME_1 = "/trigger/Sunlight_High/with/key/n5-9CvDA0F75HH-n7sANnIeGL_6rtB3x6AvQyUFA36k";  // change your EVENT-NAME and YOUR-KEY
String PATH_NAME_2 = "/trigger/Sunlight_Low/with/key/n5-9CvDA0F75HH-n7sANnIeGL_6rtB3x6AvQyUFA36k";   // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();
  lightMeter.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {


  float lux = lightMeter.readLightLevel();

  // connect to web server on port 80:
  if (client1.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  } else {  // if not connected:
    Serial.println("connection failed");
  }

  if (lux > 400) {
    // make a HTTP request:
    // send HTTP header
    client1.println("GET " + PATH_NAME_1 + queryString + " HTTP/1.1");
    client1.println("Host: " + String(HOST_NAME));
    client1.println("Connection: close");
    client1.println();  // end HTTP header

    while (client1.connected()) {
      if (client1.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client1.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client1.stop();
    Serial.println();
    Serial.println("disconnected");

    while (lux > 400) {
      lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      delay(1000);
    }
  }


  // connect to web server on port 80:
  if (client2.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  } else {  // if not connected:
    Serial.println("connection failed");
  }
  if (lux < 400) {
    // make a HTTP request:
    // send HTTP header
    client2.println("GET " + PATH_NAME_2 + queryString + " HTTP/1.1");
    client2.println("Host: " + String(HOST_NAME));
    client2.println("Connection: close");
    client2.println();  // end HTTP header

    while (client2.connected()) {
      if (client2.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client2.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client2.stop();
    Serial.println();
    Serial.println("disconnected");


    while (lux < 400) {
      lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      delay(1000);
    }
  }
}