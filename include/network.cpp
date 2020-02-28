#include <ESP8266WebServer.h>
#include <ElegantOTA.h>


ESP8266WebServer server(80);



void aato_setup(){
  server.on("/", []() {
    server.send(200, "text/plain", "Hi! I am ESP8266.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();   
}

void aato_loop(){
  server.handleClient();
}
void setup_wifi() {
    
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  aato_setup();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* reedTopic, byte* payload, unsigned int length) {



  Serial.print("Message arrived [");
 Serial.print(reedTopic);
 Serial.print("] ");

  if(reedTopic == mqtt_ota_topic){
    callbackOTA(reedTopic, payload, length);
    return;
  }

  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
 }
  Serial.println();
  payload[length] = '\0';
  mqttmessage = (char*)payload;

mqttcommand(mqttmessage);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(mqtt_sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void network_publish( const char *payload){
    Serial.print("Publish message: ");
    Serial.println(payload);
    client.publish(mqtttopic, payload);
}

void network_setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.subscribe(mqtt_ota_topic);
  client.subscribe(mqtt_sub_topic);
  client.setCallback(callback);
  network_publish("Publishing ");
  
}

void network_loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  aato_loop();
}


