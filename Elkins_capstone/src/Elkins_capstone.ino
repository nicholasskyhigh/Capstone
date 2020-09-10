/*
 * Project Elkins_capstone
 * Description: My final project for Iot Bootcamp
 * Author: Nicholas Elkins
 * Date: 9/11/2020
 */

// setup() runs once, when the device is first turned on.

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Adafruit_BME280.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Apikey.h"


#define BME280_ADDRESS  (0x77)
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme;

#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883   


TCPClient TheClient; 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

int last;
int waterlevel; //Waterlevel sensor input
int relay1, relay2, relay3;

void setup() {
  pinMode(A0, INPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  // Put initialization like pinMode and begin functions here.
  MQTT_connect();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  ping();
  // The core of your code will likely live here.

}

void MQTT_connect() {
  int8_t ret;
 
 
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
  }
  Serial.println("MQTT Connected!");
}

void ping() {
   if ((millis()-last)>30000) {
      Serial.printf("Pinging MQTT \n");
      
      if(! mqtt.ping()) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
}