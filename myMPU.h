
#include <MPU6050.h>
MPU6050 mpu;

class myMPU {
  private:
    int SCL_PIN;
    int SDA_PIN;

  public:

    myMPU(int SCLnum, int SDAnum) {
      //Setup the SCL and SDA pins:
      SCL_PIN = SCLnum;
      SDA_PIN = SDAnum;
    }

    void initializingMPU() {
      //Initialize MPU6050:
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

      // Check settings method
      checkSettings();
    }

    void checkSettings()    //in this method are settings for the MPU to work and make sleep mode work for the sensor:
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

    //Get Gyro and Accel methods by using the read methods from the MPU6050 library and return it as float:
    float getGyroX() {
      Vector rawGyro = mpu.readRawGyro();
      float num1 =  rawGyro.XAxis;
      return num1;
    }

    float getGyroY() {
      Vector rawGyro = mpu.readRawGyro();
      float num1 =  rawGyro.YAxis;
      return num1;
    }

    float getGyroZ() {
      Vector rawGyro = mpu.readRawGyro();
      float num1 =  rawGyro.ZAxis;
      return num1;
    }

    float getAccelX() {
      Vector rawAccel = mpu.readRawAccel();
      float num1 =  rawAccel.XAxis;
      return num1;
    }

    float getAccelY() {
      Vector rawAccel = mpu.readRawAccel();
      float num1 =  rawAccel.YAxis;
      return num1;
    }

    float getAccelZ() {
      Vector rawAccel = mpu.readRawAccel();
      float num1 =  rawAccel.ZAxis;
      return num1;
    }

    //=========================================================
    // The following methods are used for testing purposes to expect the values to range between specific numbers:
    bool testGyroX(float gX) {
      if (gX > 63028 && gX < 67535)
        return true;
      else
        return false;
    }

    bool testGyroY(float gY) {
      if (gY > 63308 && gY < 67652)
        return true;
      else
        return false;
    }

    bool testGyroZ(float gZ) {
      if (gZ > 63031 && gZ < 67534)
        return true;
      else
        return false;
    }
    //=========================================================
    bool testAccelX(float aX) {
      if (aX > 1944 && aX < 67517)
        return true;
      else
        return false;
    }

    bool testAccelY(float aY) {
      if (aY > 0 && aY < 67532)
        return true;
      else
        return false;
    }

    bool testAccelZ(float aZ) {
      if (aZ > 4740 && aZ < 48752)
        return true;
      else
        return false;
    }

};
