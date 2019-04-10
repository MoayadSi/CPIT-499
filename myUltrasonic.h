class MyUltrasonic {

  private:
    int Trig, Echo;

  public:
    MyUltrasonic(int Trig1, int Echo1) {
      //Determine the pins for the Trigger and the ECHO for the ultrasonic:
      Trig = Trig1;
      Echo = Echo1;
    }
    void setupUltrasonic() {
      // Ultrasonic settings:
      pinMode(Trig, OUTPUT);
      pinMode(Echo, INPUT);
    }

    //Method will return the distance by using the trigger and the echo to calculate the distance:
    float getUltrasonic() {
      float duration, distance;
      digitalWrite(Trig, LOW);
      delayMicroseconds(2);
      digitalWrite(Trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trig, LOW);
      duration = pulseIn(Echo, HIGH);
      distance = (duration / 2) / 29.1;
      delay(1000);
      return distance;
    }
};
