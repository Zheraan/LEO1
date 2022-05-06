

// Linux for Embedded Objects 1
// University of Southern Denmark

// 2022-03-24, Kjeld Jensen, First version

// Configuration
#define WIFI_SSID       "LEO1_TEAM_17"
#define WIFI_PASSWORD    "LEO_TEAM_17!"

#define MQTT_SERVER      "192.168.10.1"
#define MQTT_SERVERPORT  1883 
#define MQTT_USERNAME    "Dragomin"
#define MQTT_KEY         "Nerd"
#define MQTT_TOPIC_TEMP  "/temp"
#define MQTT_TOPIC_MOIST  "/moist"

// wifi
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
const uint32_t conn_tout_ms = 5000;

// Moisture Sensor
#include "Adafruit_seesaw.h"
Adafruit_seesaw ss;
int CapVal = 0;
int TC = 0;

// Motor Pin
#define motor 12

// mqtt
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
WiFiClient wifi_client;
Adafruit_MQTT_Client mqtt(&wifi_client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Publish temp_mqtt_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME MQTT_TOPIC_TEMP);
Adafruit_MQTT_Publish moist_mqtt_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME MQTT_TOPIC_MOIST);

// publish
#define PUBLISH_INTERVAL 5000
unsigned long prev_post_time = 0;

// debug
#define DEBUG_INTERVAL 2000
unsigned long prev_debug_time = 0;

void MoistnTemp(){
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  CapVal = capread;
  TC = tempC;
  if(CapVal < 400 ){

    digitalWrite(motor, HIGH);
   
  }

  if(CapVal > 400){

    digitalWrite(motor,LOW);
  }
}

void debug(const char *s)
{
  Serial.print (millis());
  Serial.print (" ");
  Serial.println(s);
}

void mqtt_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (! mqtt.connected())
  {
    debug("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
         Serial.println(mqtt.connectErrorString(ret));
         debug("Retrying MQTT connection in 5 seconds...");
         mqtt.disconnect();
         delay(5000);  // wait 5 seconds
    }
    debug("MQTT Connected");
  }
}

void print_wifi_status()
{
  Serial.print (millis());
  Serial.print(" WiFi connected: ");
  Serial.print(WiFi.SSID());
  Serial.print(" ");
  Serial.print(WiFi.localIP());
  Serial.print(" RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void setup()
{
  // serial
  Serial.begin(115200);
  delay(10);
  debug("Boot");

  //Motor definition
  pinMode(motor, OUTPUT);

  // wifi
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  if(WiFiMulti.run(conn_tout_ms) == WL_CONNECTED)
  {
    print_wifi_status();
  }
  else
  {
    debug("Unable to connect");
  }

  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void publish_data()
{
  char payload_temp[10];
  sprintf (payload_temp, "%ld", TC);
  char payload_moist[10];
  sprintf (payload_moist, "%ld", CapVal);
  
  Serial.print(millis());
  Serial.print(" Publishing: ");
  char buf[25];
  sprintf (buf, "Temperature: %ld", TC);
  Serial.print(buf);
  sprintf (buf, "   Moisture: %ld", CapVal);
  Serial.println(buf);

  Serial.print(millis());
  Serial.println(" Connecting...");
  if((WiFiMulti.run(conn_tout_ms) == WL_CONNECTED))
  {
    print_wifi_status();
  
    mqtt_connect();
    if (! temp_mqtt_publish.publish(payload_temp))
    {
      debug("MQTT temperature send failed");
    }
    else
    {
      debug("MQTT ok");
    }
    if (! moist_mqtt_publish.publish(payload_moist))
    {
      debug("MQTT moisture send failed");
    }
    else
    {
      debug("MQTT ok");
    }
  }
}

void loop()
{
    if (millis() - prev_post_time >= PUBLISH_INTERVAL)
    {
      debug("yeet");
      prev_post_time = millis();
      MoistnTemp();
      publish_data();
    }
   
    if (millis() - prev_debug_time >= DEBUG_INTERVAL)
    {
      prev_debug_time = millis();
      Serial.print(millis());
      Serial.print(" ");
      Serial.println(TC);
    }
}
