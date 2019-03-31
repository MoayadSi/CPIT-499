package hexapod_classifier;

import arduino.*;

public class MPU {

    private double GyroX;
    private double GyroY;
    private double GyroZ;

    public MPU() {
    }

    public String readFromSerial() {
        Arduino arduino = new Arduino("COM4", 9600); //enter the port name here, and ensure that Arduino is connected, otherwise exception will be thrown.
        arduino.openConnection();
        String readings;

        while (true) {
            readings = arduino.serialRead();

            if (readings != "") {
                System.out.println(readings);
                break;
            }
        }
        //Close Arduino Connection:
        arduino.closeConnection();
        return readings;
    }

    public double ReadGyroX(String readings) {
        try {
            double GyroXvalue;
            GyroXvalue = Double.parseDouble(readings.substring(5, 13));
            return GyroXvalue;
        } catch (NumberFormatException x) {
            System.err.println("The Gyro X value is incorrect");
        }
        return 0;
    }

    public double ReadGyroY(String readings) {
        try {
            double GyroYvalue;
            GyroYvalue = Double.parseDouble(readings.substring(18, 26));
            return GyroYvalue;
        } catch (NumberFormatException x) {
            System.err.println("The Gyro Y value is incorrect");
        }
        return 0;
    }

    public double ReadGyroZ(String readings) {
        try {
            double GyroZvalue;
            GyroZvalue = Double.parseDouble(readings.substring(31, 39));
            return GyroZvalue;
        } catch (NumberFormatException x) {
            System.err.println("The Gyro X value is incorrect");
        }
        return 0;
    }

    public double getGyroX() {
        return GyroX;
    }

    public void setGyroX(double GyroX) {
        this.GyroX = GyroX;
    }

    public double getGyroY() {
        return GyroY;
    }

    public void setGyroY(double GyroY) {
        this.GyroY = GyroY;
    }

    public double getGyroZ() {
        return GyroZ;
    }

    public void setGyroZ(double GyroZ) {
        this.GyroZ = GyroZ;
    }

}
