#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include "myMPU.h";
#include "myUltrasonic.h";

//For the Firebase:
#define FIREBASE_HOST "myproject-d9975.firebaseio.com"
#define FIREBASE_AUTH "qMu5aHbg1qmpurM6LkJa105gsTQCGolXCPvmxnlp"
#define WIFI_SSID "AndroidAPEADC"
#define WIFI_PASSWORD "kyxf1580"

//For the LED:
// set pin numbers:
const int buttonPin = 15;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables:
bool buttonState;         // variable for reading the pushbutton status
int counter;
myMPU mpu1(D6, D7);
MyUltrasonic MyUS(5, 4);

void setup() {
  Serial.begin(9600);
  MyUS.setupUltrasonic();
  //--------------------------------------------------------------------
  //Button and LED setup:
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW); // Pullup functionality
  //--------------------------------------------------------------------

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //--------------------------------------------------------------------
  mpu1.initializingMPU();
}

void loop() {

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  float xGyro = mpu1.getGyroX();
  float yGyro = mpu1.getGyroY();
  float zGyro = mpu1.getGyroZ();

  float Xacc = mpu1.getAccelX();
  float Yacc = mpu1.getAccelY();
  float Zacc = mpu1.getAccelZ();

  float USvalue = MyUS.getUltrasonic();
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState || counter > 0) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    //Print values on the serial:
    Serial.print(" Xraw = ");
    Serial.print(xGyro);
    Serial.print(" Yraw = ");
    Serial.print(yGyro);
    Serial.print(" Zraw = ");
    Serial.println(zGyro);

    //Print the accelerometer values on the serial:
    Serial.println();
    Serial.print(" Xraw accel = ");
    Serial.print(Xacc);
    Serial.print(" Yraw accel = ");
    Serial.print(Yacc);
    Serial.print(" Zraw accel = ");
    Serial.println(Zacc);
    Serial.println("---------------------------------------------");
    
    //Ultrasonic value will be determined:
    Serial.print("Centimeter: ");
    Serial.println(USvalue);
    Serial.println("---------------------------------------------");
    Serial.println("---------------------------------------------");

    //===========================================================================================
    //For Testing purposes only
//    bool GyroXtest = mpu1.testGyroX(xGyro);
//    Serial.println("GyroX Test = ");
//    Serial.println(GyroXtest);
//    bool GyroYtest = mpu1.testGyroY(yGyro);
//    Serial.println("GyroY Test = ");
//    Serial.println(GyroYtest);
//    bool GyroZtest = mpu1.testGyroZ(zGyro);
//    Serial.println("GyroZ Test = ");
//    Serial.println(GyroZtest);
//    bool AccelXtest = mpu1.testAccelX(Xacc);
//    Serial.println("AccelX Test = ");
//    Serial.println(AccelXtest);
//    bool AccelYtest = mpu1.testAccelY(Yacc);
//    Serial.println("AccelY Test = ");
//    Serial.println(AccelYtest);
//    bool AccelZtest = mpu1.testAccelZ(Zacc);
//    Serial.println("AccelZ Test = ");
//    Serial.println(AccelZtest);
    //===========================================================================================

    //===========================================================================================
    //Upload the gyroscope values on Firebase with error handling:
    Firebase.pushFloat("GyroX", xGyro);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting gyro X failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("GyroY", yGyro);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting gyro Y failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("GyroZ", zGyro);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting gyro Z failed:");
      Serial.println(Firebase.error());
      return;
    }
    //===========================================================================================
    //Upload the accelerometer values on the Firebase with error handling:
    Firebase.pushFloat("accelX", Xacc);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting accel X failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("accelY", Yacc);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting accel Y failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("accelZ", Zacc);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting accel Z failed:");
      Serial.println(Firebase.error());
      return;
    }
    //===========================================================================================
    //Upload Ultrasonic value to the firebase:
    Firebase.pushFloat("UltrasonicCM", USvalue);
    // handle error
    if (Firebase.failed()) {
      Serial.println("setting ultrasonic value failed:");
      Serial.println(Firebase.error());
      return;
    }
    //===========================================================================================

    //Use the counter to enter inside this if statement 10 times:
    counter++;

    //If the counter's value is 10 reset the value and wait for another button press:
    if (counter > 9) {
      counter = 0;
    }
  }

  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }

}
