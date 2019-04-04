#include <ESP8266WiFi.h>
#include <Wire.h>
#include "myMPU.h";
#include "myUltrasonic.h";
#include <FirebaseArduino.h>


//For the Firebase:
#define FIREBASE_HOST "myproject-d9975.firebaseio.com"
#define FIREBASE_AUTH "qMu5aHbg1qmpurM6LkJa105gsTQCGolXCPvmxnlp"
#define WIFI_SSID "Turokami"
#define WIFI_PASSWORD "12345678"


//For the LED:
// set pin numbers:
const int buttonPin = 15;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables:
bool buttonState;         // variable for reading the pushbutton status
int counter;
myMPU mpu1(D6, D7);
MyUltrasonic MyUS(5, 4);
String data,trainingOrOperational;

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

  
  Serial.setTimeout(50);
  mpu1.initializingMPU();

}

void loop() {

if(Serial.available()>0){                 //if data has been written to the Serial stream

  data=Serial.readString();
    

  if(data.substring(0,2) == "T"){

  Serial.print("The_mode_changed_to_Training_mod_!!!");
    while(true){

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
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
if(Serial.available()>0){
  trainingOrOperational = Serial.readString();

  if(trainingOrOperational.substring(0,2) == "T"){
    Serial.print("You_are_already_in_the_Training_mod_!!!");
    
    }

    else if(trainingOrOperational.substring(0,2) == "O"){
      Serial.print("The_mode_changed_to_Operational_mod_!!!");
      break;
      }
  }
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

     //*********************************************************************************************************
    }
  
  }

  else if(data.substring(0,2) == "O"){
  Serial.print("You_are_already_in_the_Operational_mod_!!!");
  }

    else if(data.substring(0,2) == "1"){
  Serial.print("The_Position_is_Forward_!!!)");
  }  
  
  
  else if(data.substring(0,2) == "2"){
  Serial.print("The_Position_is_Right_!!!");
  } 
  
  
  
  else if(data.substring(0,2) == "3"){
  Serial.print("The_Position_is_Left_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "4"){
  Serial.print("The_Position_is_Backward_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "5"){
  Serial.print("The_Position_is_Forward-Right_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "6"){
  Serial.print("The_Position_is_Forward-Left_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "7"){
  Serial.print("The_Position_is_Backward-Right_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "8"){
  Serial.print("The_Position_is_Backward-Left_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "9"){
  Serial.print("The_Position_is_Sloped_Forward_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "10"){
  Serial.print("The_Position_is_Sloped_Right_!!!");
  }  
  
  
  
  else if(data.substring(0,2) == "11"){
  Serial.print("The_Position_is_Sloped_Left_!!!");
  }  
  
  
  else if(data.substring(0,2) == "12"){
  Serial.print("The_Position_is_Sloped_Backward_!!!");
  }  
  
  
  
  
  else if(data.substring(0,2) == "13"){
  Serial.print("The_Position_is_Fliped_!!!");
  }


  else if(data.substring(0,2) == "R"){

    //Get the gyro values:
  float xGyro = mpu1.getGyroX();
  float yGyro = mpu1.getGyroY();
  float zGyro = mpu1.getGyroZ();

  //Print the values on the Serial:

  
  Serial.print("GyroX");
  Serial.print(xGyro);
  Serial.print("GyroY");
  Serial.print(yGyro);
  Serial.print("GyroZ");
  Serial.print(zGyro);
  
    
    
    }

    else{
      
      Serial.print("No_such_command_available_!!!");
      
      }


  

  }
  
  //Get the gyro values:

  float xGyro = mpu1.getGyroX();
  float yGyro = mpu1.getGyroY();
  float zGyro = mpu1.getGyroZ();
  

  
  delay(10000);
}
