#include <SoftwareSerial.h>
#include <espduino.h>
#include <mqtt.h>

SoftwareSerial debugPort(2, 3); // RX, TX
ESP esp(&Serial, &debugPort, 4);
MQTT mqtt(&esp);
boolean wifiConnected = false;

/* Ultrasonic Sensor */
int trigPin = 12;
int echoPin = 11;
//


void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      debugPort.println("WIFI CONNECTED");
      mqtt.connect("192.168.43.93", 1883, false);
      wifiConnected = true;
      //or mqtt.connect("host", 1883); /*without security ssl*/
    } else {
      wifiConnected = false;
      mqtt.disconnect();
    }
    
  }
}

void mqttConnected(void* response)
{
  debugPort.println("Connected");
  mqtt.subscribe("/sws/test1"); //or mqtt.subscribe("topic"); /*with qos = 0*/
  //mqtt.subscribe("/topic/1");
  //mqtt.subscribe("/topic/2");
  //mqtt.publish("/topic/0", "data0");

}
void mqttDisconnected(void* response)
{

}
void mqttData(void* response)
{
  RESPONSE res(response);

  debugPort.print("Received: topic=");
  String topic = res.popString();
  debugPort.println(topic);

  debugPort.print("data=");
  String data = res.popString();
  debugPort.println(data);

  if( topic == "/sws/test1"){
    if(data=="hi"){
      mqtt.publish("/sws/test2", "From Server");
    }
  }
  

}
void mqttPublished(void* response)
{

}
void setup() {

  /* Ultrasonic Sensor */
  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT); 
  
  Serial.begin(19200);
  debugPort.begin(19200);
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  debugPort.println("ARDUINO: setup mqtt client");
  if(!mqtt.begin("DVES_duino", "admin", "Isb_C4OGD4c3", 20, 1)) {
    debugPort.println("ARDUINO: fail to setup mqtt");
    while(1);
  }


  debugPort.println("ARDUINO: setup mqtt lwt");
  mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

/*setup mqtt events */
  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.disconnectedCb.attach(&mqttDisconnected);
  mqtt.publishedCb.attach(&mqttPublished);
  mqtt.dataCb.attach(&mqttData);

  /*setup wifi*/
  debugPort.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  esp.wifiConnect("LGMobile","jugal1234");


  debugPort.println("ARDUINO: system started");
}

void loop() {
  esp.process();
  if(wifiConnected) {
    //char tmp[255];
    long distance = getDistance();
    //sprintf(tmp, "%ld", distance);

    //String thisString = String(distance);

    char temp[10];
    ltoa(9L
    
    ,temp,10);
    
    mqtt.publish("/sws/test2",temp);
  }
}    


long getDistance()
{
  long t = 0, h = 0, hp = 0;
  
  // Transmitting pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Waiting for pulse
  t = pulseIn(echoPin, HIGH);// t is length of pulse in microsecond
  //works on pulses from 10 microsecond to 3 min length
  
  // Calculating distance 
  h = t / 58;//h is hight in CM //if need hight in inch then divide by 148
  
  //h = h - 6;  // offset correction
  //h = 50 - h;  // water height, 0 - 50 cm
  //hp = 2 * h;  // distance in %, 0-100 %

  
  // Sending to computer
  Serial.print(h);
  //Serial.print(hp);
  // Serial.print(" cm\n");
  Serial.print("\n");
  
  delay(1000);
  return h;
}
