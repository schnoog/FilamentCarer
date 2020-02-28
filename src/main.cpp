#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "credentials.cpp"

String MyVERSION="MQTT-Hum-Balance-Master-v0.95";
const char* host = "MQTT-Hum-Balance-Master-v0.95";

unsigned long LastMoment;
unsigned long MainLoopWork = 60000;

//////// Network & MQTTTopics

const char* mqtt_server = "192.168.178.27";
const char* mqtttopic = "3d";
const char* mqtt_pub_topic = "3d";
const char* mqtt_sub_topic = "3d/CMD";
const char* mqtt_ota_topic = "3d/OTA";

WiFiClient espClient;
PubSubClient client(espClient);

//////// PINS
const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;

#define RST_PIN         D3           // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
uint8_t DHTPin = D4;
int PumpPin = D0;
/////// OTHERS
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
String mqttmessage;
float HumUpperLimit = 33.3;
float HumLowerLimit = 25.7;

/////// DEBUGs

bool IsDebug = true;

#include "headers.cpp"

#include "mqtt_commands.cpp"
#include "network.cpp"
#include "balance.cpp"
#include "rfid_id.cpp"
#include "humidity.cpp"
#include "mqtt_ota.cpp"

void setup() {
    LastMoment = millis();
    Serial.begin(9600);
    balance_setup();
    network_setup();
    rfid_setup();
    humidity_setup();
}

void loop() {
  network_loop();
  humidityControl(false); 
  unsigned long tmpx = millis()- MainLoopWork;
  if(LastMoment < tmpx){
    LastMoment = millis();
    explain();
    //run every MainLoopWork

  }



//  rfid_setup()
}