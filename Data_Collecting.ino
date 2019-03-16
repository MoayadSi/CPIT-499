#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <MPU6050.h>

//For the Firebase:
#define FIREBASE_HOST "myproject-d9975.firebaseio.com"
#define FIREBASE_AUTH "qMu5aHbg1qmpurM6LkJa105gsTQCGolXCPvmxnlp"
#define WIFI_SSID "AndroidAPEADC"
#define WIFI_PASSWORD "kyxf1580"

//For the LED:
// set pin numbers:
const int buttonPin = 15;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables will change:
bool buttonState;         // variable for reading the pushbutton status
//bool state;
int counter;


//For MPU:
MPU6050 mpu;
int SCL_PIN = D6;
int SDA_PIN = D7;


void setup() {
  Serial.begin(9600);

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
  //Initialize MPU6050
  Serial.println("Initialize MPU6050");
  while (!mpu.beginSoftwareI2C(SCL_PIN, SDA_PIN, MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);

  // Check settings
  checkSettings();
}

void checkSettings()
{
  Serial.println();
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  Serial.print(" * Clock Source:      ");
  switch (mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  Serial.print(" * Gyroscope:         ");
  switch (mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  }
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());
  Serial.println();

  Serial.print(" * Accelerometer:         ");
  switch (mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());

  Serial.println();
}


void loop() {
  Vector rawGyro = mpu.readRawGyro();
  Vector rawAccel = mpu.readRawAccel();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  float Xval = rawGyro.XAxis;
  float Yval = rawGyro.YAxis;
  float Zval = rawGyro.ZAxis;

  float Xacc = rawAccel.XAxis;
  float Yacc = rawAccel.YAxis;
  float Zacc = rawAccel.ZAxis;


  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState || counter > 0) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    //Print values on the serial:
    Serial.print(" Xraw = ");
    Serial.print(Xval);
    Serial.print(" Yraw = ");
    Serial.print(Yval);
    Serial.print(" Zraw = ");
    Serial.println(Zval);

    //Print the accelerometer values on the serial:
    Serial.println();
    Serial.print(" Xraw accelerometer = ");
    Serial.print(Xacc);
    Serial.print(" Yraw accelerometer ");
    Serial.print(Yacc);
    Serial.print(" Zraw accelerometer  ");
    Serial.print(Zacc);

    //Upload the gyroscope values on Firebase with error handling:
    Firebase.pushFloat("numberX", Xval);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number X failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("numberY", Yval);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number Y failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("numberZ", Zval);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number Z failed:");
      Serial.println(Firebase.error());
      return;
    }


    //Upload the accelerometer values on the Firebase with error handling:
    Firebase.pushFloat("accelX", Xacc);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number X failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("accelY", Yacc);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number Y failed:");
      Serial.println(Firebase.error());
      return;
    }
    Firebase.pushFloat("accelZ", Zacc);
    // handle error
    if (Firebase.failed()) {
      Serial.print("setting number Z failed:");
      Serial.println(Firebase.error());
      return;
    }

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
