#include "DHT.h"
#define DHTTYPE DHT22 


DHT dht(DHTPin, DHTTYPE);                

bool HumControlActive;
float Temperature;
float Humidity;
int PumpStatus = 0;

unsigned long previousMillis = 0;  
const long interval = 60000;  
int MaxOffCycles = 30;
int OffCycle = 0;


void humidityControl(bool bolForce) {
    
    long MYINT = interval;
    if(bolForce) {MYINT  =  1;}
    unsigned long currentMillis = millis();
    if( previousMillis < 1){
      currentMillis = MYINT + 2;
    }
    if (currentMillis - previousMillis >= MYINT)
    {
            previousMillis = currentMillis;
          Temperature = dht.readTemperature(); // Gets the values of the temperature
          Humidity = dht.readHumidity(); // Gets the values of the humidity 
          String ControlState = "inactive";
          if(HumControlActive){
              ControlState = "active";
              if(Humidity > HumUpperLimit){
              digitalWrite(PumpPin, HIGH);
              PumpStatus = 1;
              }
              if(Humidity < HumLowerLimit){
                digitalWrite(PumpPin, LOW);
                PumpStatus = 0;
              }
          }
          Serial.println("---------------------");
          Serial.println(Temperature);
          Serial.println(Humidity);
          Serial.print("pumpe: ");
          Serial.println(PumpStatus);   

        
        snprintf (msg, MSG_BUFFER_SIZE, "LIMITS: %.2f - %.2f", HumLowerLimit,HumUpperLimit);
        Serial.println(msg);
        snprintf (msg, MSG_BUFFER_SIZE, "%.2f", HumLowerLimit);
        network_publish(msg,"settings/humidity/lowerlimit");
        snprintf (msg, MSG_BUFFER_SIZE, "%.2f", HumUpperLimit);
        network_publish(msg,"settings/humidity/upperlimit");




        String retDataH = (String)Humidity;  //Specify request destination
        snprintf (msg, MSG_BUFFER_SIZE, "HUMIDITY: %s", retDataH.c_str());
        Serial.println(msg);
        //network_publish(msg);

        snprintf (msg, MSG_BUFFER_SIZE, "%s", retDataH.c_str());
        network_publish(msg,"filament/humidity");

        retDataH = (String)Temperature;  //Specify request destination
        snprintf (msg, MSG_BUFFER_SIZE, "TEMPERATURE: %s", retDataH.c_str());
        Serial.println(msg);
        //network_publish(msg);

        snprintf (msg, MSG_BUFFER_SIZE, "%s", retDataH.c_str());
        network_publish(msg,"filament/temperature");

        retDataH = (String)PumpStatus;  //Specify request destination
        snprintf (msg, MSG_BUFFER_SIZE, "PUMP: %s", retDataH.c_str());
        Serial.println(msg);
        //network_publish(msg);

        snprintf (msg, MSG_BUFFER_SIZE, "%s", retDataH.c_str());
        network_publish(msg,"filament/pump");
        
        snprintf (msg, MSG_BUFFER_SIZE, "CONTROLSTATE: %s", ControlState.c_str());
        Serial.println(msg);
        //network_publish(msg);
        snprintf (msg, MSG_BUFFER_SIZE, "%s", ControlState.c_str());
        network_publish(msg,"filament/controlstate");


    }    
 
}



void humidity_setup(){
        HumControlActive = true;
        pinMode(DHTPin, INPUT);
        pinMode(PumpPin, OUTPUT);
        dht.begin();
          Humidity = dht.readHumidity(); // Gets the values of the humidity 
  if(Humidity > HumLowerLimit){
    digitalWrite(PumpPin, HIGH);
    PumpStatus = 1;
  } 
}


void humidity_activate_control(){
  HumControlActive = true;
  String ControlState = "active";
  snprintf (msg, MSG_BUFFER_SIZE, "CONTROLSTATE: %s", ControlState.c_str());
  Serial.println(msg);
  snprintf (msg, MSG_BUFFER_SIZE, "%s", ControlState.c_str());
  network_publish(msg,"filament/controlstate");  
}

void humidity_deactivate_control(){
  HumControlActive = false;
  String ControlState = "inactive";
  snprintf (msg, MSG_BUFFER_SIZE, "CONTROLSTATE: %s", ControlState.c_str());
  Serial.println(msg);
  snprintf (msg, MSG_BUFFER_SIZE, "%s", ControlState.c_str());  
  network_publish(msg,"filament/controlstate");  
}

void humidity_pump_start(){
    digitalWrite(PumpPin, HIGH);
    PumpStatus = 1;
    String retDataH = (String)PumpStatus;  //Specify request destination
    snprintf (msg, MSG_BUFFER_SIZE, "PUMP: %s", retDataH.c_str());
    Serial.println(msg);
    snprintf (msg, MSG_BUFFER_SIZE, "%s", retDataH.c_str());
    network_publish(msg,"filament/pump");    
}

void humidity_pump_stop(){
    digitalWrite(PumpPin, LOW);
    PumpStatus = 0;
    String retDataH = (String)PumpStatus;  //Specify request destination
    snprintf (msg, MSG_BUFFER_SIZE, "PUMP: %s", retDataH.c_str());
    Serial.println(msg);
    snprintf (msg, MSG_BUFFER_SIZE, "%s", retDataH.c_str());
    network_publish(msg,"filament/pump");
}