#include "HX711.h"

HX711 scale;

bool balance_initialized = false;

void balance_read(){
    if(! balance_initialized){balance_setup();}
    int cnt=0;
    if (!scale.is_ready()){
        if(cnt < 10){
            delay(100);
            cnt++;
        }
    }    
    if (scale.is_ready()) {
        snprintf (msg, MSG_BUFFER_SIZE, "Weight %ld", scale.read_average(20));
        network_publish(msg);
        
  } else {
        Serial.println("HX711 not found.");
  }
}

void balance_setzero(){
    if (scale.is_ready()) {
    scale.set_scale();
    scale.tare();
    }
}

void balance_raw(){
    if(! balance_initialized){balance_setup();}
    int cnt=0;
    if (!scale.is_ready()){
        if(cnt < 10){
            delay(100);
            cnt++;
        }
    }
    if (scale.is_ready()) {
        Serial.print("HX711 raw: ");
        snprintf (msg, MSG_BUFFER_SIZE, "Weightunits %ld", scale.get_units(10));
        Serial.println(msg);
        network_publish(msg);
    }
}

void balance_setup(){
    balance_initialized = true;
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

