#include <ESP8266WiFi.h>
#include <Wire.h>
#include "myMPU.h";
#include "myUltrasonic.h";
#include <FirebaseArduino.h>
#line 2 "myMPU.h"
#include <ArduinoUnit.h>

#define WIFI_SSID "Turokami"
#define WIFI_PASSWORD "12345678"


myMPU mpu1(D6, D7);
MyUltrasonic MyUS(5, 4);


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

test(GyroX)
{
  
  float xGyro = mpu1.getGyroX();
  
  assertMoreOrEqual(xGyro,63028);
  assertLessOrEqual(xGyro,67535);

 
}


test(GyroY)
{
  
  float yGyro = mpu1.getGyroY();
  
  assertMoreOrEqual(yGyro,63308);
  assertLessOrEqual(yGyro,67652);
  
}


test(GyroZ)
{
  
  float zGyro = mpu1.getGyroZ();
  
  assertMoreOrEqual(zGyro,63031);
  assertLessOrEqual(zGyro,67534);
  
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void setup() {
  Serial.begin(9600);
  MyUS.setupUltrasonic();


  //Connect to wifi setup.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());



  
  Serial.setTimeout(50);
  mpu1.initializingMPU();

}

void loop() {

 Test::run();

  
 
 
}
