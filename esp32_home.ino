#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "conf.h"

const size_t CAPACITY = JSON_OBJECT_SIZE(2);
StaticJsonDocument<CAPACITY> doc;

DHT dht(12, DHT11);

float temperature = 0;
float humidity = 0;
float last_humidity = 0;
float last_temperature;

void setup() {
  dht.begin();
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
 delay(5000);
  getWeather();

  if(last_temperature != temperature || last_humidity != humidity){
    if(WiFi.status()== WL_CONNECTED){
          WiFiClient client;
          HTTPClient http;        
          
          http.begin(client, serverName);              
        
          http.addHeader("Content-Type", "application/json");

          String json;
          JsonObject object = doc.to<JsonObject>();
          object["humidity"] = humidity;
          object["temperature"] = temperature;

          serializeJson(object, json);                
          
          int httpResponseCode = http.POST(json);  
        
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
            
          // Free resources
          http.end();
        }
        else {
          Serial.println("WiFi Disconnected");
        }
      last_humidity = humidity;
      last_temperature = temperature;
    }
}

void getWeather(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print(temperature);
  
}
