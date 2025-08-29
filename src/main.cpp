#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>
#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

#include "CTBot.h"
#include <ArduinoJson.h>

#define led 13
// int led=13;

CTBot myBot;
TBMessage msg;
// String ssid  = "YatishPhone"    ; // Enter WIFI Username
// String pass  = "12345678"; // Enter WIFI Password

String token = "____Enter yours___"   ; // Enter the token from Telegram Bor
String ssid  = "eduroam"    ; // Enter WIFI Username
String pass  = "___enter yours___"; // Enter WIFI Password
String EAP_ANONYMOUS_IDENTITY = "anonymous@uni-freiburg.de"; //anonymous@example.com, or you can use also nickname@example.com
String EAP_IDENTITY = "yy50@uni-freiburg.de";

float thresholdTemperature = 37.0;
float alertTemperature = 37.5;
Adafruit_MLX90614 mlx;
// byte mac[6];



// #define led_wifi 1

/* #define EAP_ANONYMOUS_IDENTITY "anonymous@uni-freiburg.de" //anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "yy50@uni-freiburg.de" //nickname@example.com, at some organizations should work nickname only without realm, but it is not recommended
#define EAP_PASSWORD "Yatish@1998" //password for eduroam account */

//SSID NAME
// const char* ssid = "eduroam"; // eduroam SSID

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  // pinMode(led_wifi, OUTPUT);
/*   delay(10);
  Serial.print(F("Connecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect from WiFi to set new WiFi connection
  WiFi.mode(WIFI_STA); //init wifi mode */
  // WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD, test_root_ca); //with CERTIFICATE

/*   WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD); //WITHOUT CERTIFICATE - WORKING WITH EXCEPTION ON RADIUS SERVER

  while (WiFi.status() != WL_CONNECTED) {
    // digitalWrite(led_wifi, HIGH);
    // delay(250);
    Serial.print(F("."));
    // digitalWrite(led_wifi, LOW);
    delay(250);
  }
  Serial.println("");
  Serial.println(F("WiFi is connected!"));
  Serial.println(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[0], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.println(mac[5], HEX); */

  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, pass);
  myBot.setTelegramToken(token);
  if (myBot.testConnection()){
   
    Serial.println("\ntestConnection OK");
  }
  else
  {
    Serial.println("\ntestConnection NOK");
  }
  mlx.begin();
  // float newEmissivity = 1;  // New emissivity value to set
  // mlx.writeEmissivity(newEmissivity);
}


void loop(){ 
  // digitalWrite(led_wifi, WiFi.status() == WL_CONNECTED);

  float tempObjC = mlx.readObjectTempC();
  float tempAmbC = mlx.readAmbientTempC();

  Serial.print("Object Temperature: ");
  Serial.print(tempObjC);
  Serial.print(" °C\t");
  Serial.print("Ambient Temperature: ");
  Serial.print(tempAmbC);
  Serial.println(" °C");

  String ambient = "Ambient: " + String(tempAmbC) + "°C \n";
  String object = "Object: " + String(tempObjC) + "°C \n";
  String alert = "ALERT! Object very hot: " + String(tempObjC) + "°C \n";

  if (myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("Ambient")) {    
     myBot.sendMessage(msg.sender.id, ambient);
    }
    if (msg.text.equalsIgnoreCase("Object")) {    
          myBot.sendMessage(msg.sender.id, object); 
    }

  }

  if (tempObjC>thresholdTemperature){    
    digitalWrite(led,LOW);

    if (tempObjC>alertTemperature){
      myBot.sendMessage(msg.sender.id, alert);
    }
  } else {
    // delay(500);
    digitalWrite(led,HIGH);
    delay(10);
  }


}