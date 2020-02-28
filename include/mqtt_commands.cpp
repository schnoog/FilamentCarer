
void mqttcommand(String StrCommand){
//
if(IsDebug){
    snprintf (msg, MSG_BUFFER_SIZE, "RECEIVED MESSAGE: %s", StrCommand.c_str());
    Serial.println(msg);
    network_publish(msg);   

}

/**
 *  CMD
 */
  if(StrCommand == "balance"){
    balance_read();
    while(StrCommand != "balance"){
    break;
    }
  } 
/**
 *  CMD
 */
  if(StrCommand == "getid"){
    handleRFID();
    while(StrCommand != "getid"){
      break;
    }
  }
/**
 *  CMD
 */
  if(StrCommand == "checkhum"){
    humidityControl(true);
    while(StrCommand != "checkhum"){
      break;
    }
  }

/**
 *  CMD
 */
  if(StrCommand == "disablehumcontrol"){
    humidity_deactivate_control();
    while(StrCommand != "disablehumcontrol"){
      break;
    }
  }

/**
 *  CMD
 */
  if(StrCommand == "enablehumcontrol"){
    humidity_activate_control();
    while(StrCommand != "enablehumcontrol"){
      break;
    }
  }
/**
 *  CMD
 */
  if(StrCommand == "startpump"){
    humidity_pump_start();
    while(StrCommand != "startpump"){
      break;
    }
  }
/**
 *  CMD
 */
  if(StrCommand == "stoppump"){
    humidity_pump_stop();
    while(StrCommand != "stoppump"){
      break;
    }
  }
/**
 *  CMD
 */
  if(StrCommand == "ABC"){
    humidityControl(true);
    while(StrCommand != "ABC"){
      break;
    }
  }


  if(StrCommand.length() > 7){
      String Subs = StrCommand.substring(0,6);
      Serial.print("extrahierte subs: ");
      Serial.println(Subs);
      
      if(Subs == "setmin"){
          float newval = StrCommand.substring(7).toFloat();
          Serial.print("Set new min to:");
          Serial.println(newval);
          HumLowerLimit = newval;
      }
      if(Subs == "setmax"){
          float newval = StrCommand.substring(7).toFloat();
          Serial.print("Set new max to:");
          Serial.println(newval);  
          HumUpperLimit = newval;        
      }



  }




//
}


/**
 * 
 */
unsigned long LastEX;
void explain(){
  unsigned long TT = millis();
  if ( LastEX + 10000 > TT) return;
  LastEX = millis();

  String MyMessage;
    MyMessage = WiFi.localIP().toString();   
    snprintf (msg, MSG_BUFFER_SIZE, "KA-IP: %s", MyMessage.c_str());
    network_publish(msg);

    snprintf (msg, MSG_BUFFER_SIZE, "KA-UPDATE-URL: http://%s/update", MyMessage.c_str());
    network_publish(msg);

    MyMessage = "commands on TOPIC " + (String)mqtt_sub_topic;   
    snprintf (msg, MSG_BUFFER_SIZE, "KA-IP: %s", MyMessage.c_str());
    network_publish(msg);

    MyMessage = "Available Commands: balance,getid,checkhum";   
    snprintf (msg, MSG_BUFFER_SIZE, "KA-IP: %s", MyMessage.c_str());
    network_publish(msg);

    MyMessage = "AC: disablehumcontrol,enablehumcontrol,startpump";   
    snprintf (msg, MSG_BUFFER_SIZE, "KA-IP: %s", MyMessage.c_str());
    network_publish(msg);

    MyMessage = "AC: stoppump,setmin <float>,setmax <float>";   
    snprintf (msg, MSG_BUFFER_SIZE, "KA-IP: %s", MyMessage.c_str());
    network_publish(msg);
  
    snprintf (msg, MSG_BUFFER_SIZE, "KA-Version: %s", MyVERSION.c_str());
    network_publish(msg);


}