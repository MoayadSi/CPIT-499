#include <ESP8266WiFi.h>
#include <Wire.h>
#include "myMPU.h";

myMPU mpu1(D6, D7);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  mpu1.initializingMPU();

}

void loop() {
  
  //Get the gyro values:
  float xGyro = mpu1.getGyroX();
  float yGyro = mpu1.getGyroY();
  float zGyro = mpu1.getGyroZ();

  //  float Xacc = mpu1.getAccelX();
  //  float Yacc = mpu1.getAccelY();
  //  float Zacc = mpu1.getAccelZ();
  
  //Print the values on the Serial:
  Serial.print("GyroX");
  Serial.print(xGyro);
  Serial.print("GyroY");
  Serial.print(yGyro);
  Serial.print("GyroZ");
  Serial.print(zGyro);
  
//  Serial.print("AccelX");
//  Serial.print(Xacc);
//  Serial.print("AccelY");
//  Serial.print(Yacc);
//  Serial.print("AccelZ");
//  Serial.print(Zacc);
  
  delay(10000);
}
