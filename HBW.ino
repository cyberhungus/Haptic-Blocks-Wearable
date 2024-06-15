#include <ESP32Servo.h>


#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <HTTPClient.h>
bool isLocal = true;

const int vibmotors[] = {14, 27, 26, 25,19,18,5,17};
const int servopins[]  = {12, 13};

const int vibmotorslength = sizeof(vibmotors);


const int servoslength = sizeof(servopins);

Servo servos[servoslength];


const int minPosition = 90;
const int maxPosition = 175;


const int PinLocalToggle = 15; 


const int minIntensity = 170;


const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXXX";


const char* ssidLocal = "HapticNetwork";
const char* passwordLocal = "testtest";




AsyncWebServer server(80);






void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  //this section handles all the pin stuff
  for (int i = 0; i < vibmotorslength; i++) {
    pinMode(vibmotors[i], OUTPUT);
  }
  for (int i = 0; i < servoslength; i++) {
    servos[i].setPeriodHertz(50); // standard 50 hz servo
    servos[i].attach(servopins[i], 1000, 2000);
    servos[i].write(170);
    delay(2000);
    servos[i].write(90);
    


  }





  if (!isLocal) {

    //this section handles wifi
    WiFi.mode(WIFI_AP);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());




  }
  else {

    //starte im Access Point Modus (esp = router)
    WiFi.softAP(ssidLocal, passwordLocal);
  
  }


  //Index Seite zum Testen, ob das Armband Online ist.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "Online");
  });





  //erlaubt das Abschalten des Vibrationsmotors
  server.on("/vibration", HTTP_GET, vibrateFunc);

  //erlaubt das Abschalten des Vibrationsmotors
  server.on("/servo", HTTP_GET, servoFunc);

 



  server.begin();
}

void loop() {
  delay(1);
}
