/*
 * Project Elkins_capstone
 * Description: My final project for Iot Bootcamp
 * Author: Nicholas Elkins
 * Date: 9/11/2020
 */


#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Adafruit_BME280.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "DHT22Gen3_RK.h"
#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
#include "Apikey.h"


#define BME280_ADDRESS  (0x77)
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme;

#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883 
//this is a github test  


TCPClient TheClient; 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Subscribe activate = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/activate"); 
Adafruit_MQTT_Subscribe turnon = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/turnon");

int last;
int waterlevel; //Waterlevel sensor input
int relay1 = D2, relay2 = D3, relay3 = D4; //sets up the relays for each tec
int pushbutton = D9;

void setup() {
  pinMode(A0, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  MQTT_connect();
  begin();


}

void loop() {
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  ping();
  display.clearDisplay();
  display.println("hello world");
  display.display();
  

}

void onoff() { 

}

void climateread() {

}

void begin() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  bme.begin();
  display.display();
  delay(750);
}

void MQTT_connect() { //connection to adafruit.io
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

void ping() {  //pings adafruit.io to make sure  connection is active
   if ((millis()-last)>30000) {
      Serial.printf("Pinging MQTT \n");
      
      if(! mqtt.ping()) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
}