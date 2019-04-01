package hexapod_classifier;

import java.sql.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import org.rosuda.JRI.*;
import arduino.*;

public class Hexapod_Classifier {

    private static final String USERNAME = "root";
    private static final String PASSWORD = "";
    private static final String CONN_STRING = "jdbc:mysql://127.0.0.1/Hexapod_CF";

    public static void main(String[] args) {
        Connection conn = null;
        try {
            //Create connection to the local DB and print a message:
            conn = DriverManager.getConnection(CONN_STRING, USERNAME, PASSWORD);
            System.out.println("Connecttion to DB is successful");

            //Create a statment and a resultset:
            Statement stmt = conn.createStatement();
            ResultSet rs = null;

            //=================================================================
            //String createTable = "CREATE TABLE coefficients (Intercept INT,GyroX INT, GyroY INT, GyroZ INT, PRIMARY KEY (Intercept))";
            //String insertInto = "INSERT INTO coefficients (Intercept,GyroX, GyroY, GyroZ) values (44523,2343,52334,223)";
            //stmt.execute(createTable);
            //=================================================================
            //Identify variables:
            String mySelectStmt;
            int index = 0;
            double GyroXvalues[] = new double[2600];
            double GyroYvalues[] = new double[2600];
            double GyroZvalues[] = new double[2600];
            double Position[] = new double[2600];

            //Write the select statment and save it in resultset:
            mySelectStmt = "select * from training_data";
            rs = stmt.executeQuery(mySelectStmt);

            //Move the cursor through the resultset and save the values in the proper array:
            while (rs.next()) {
                GyroXvalues[index] = rs.getInt(2);
                GyroYvalues[index] = rs.getInt(3);
                GyroZvalues[index] = rs.getInt(4);
                Position[index] = rs.getInt(5);

                //Print the values for testing:
                //System.out.println(index + " " + GyroXvalues[index]);
                index++;
            }

            //=================================================================
            //Connect netbeans with R
            if (!Rengine.versionCheck()) {
                System.err.println("Java version mismatch.");
                System.exit(1);
            }
            //Create the Rengine that will allow us to use R functions
            Rengine re = new Rengine(new String[]{"--no-save"}, false, null);
            if (!re.waitForR()) {
                System.out.println("Cannot load R");
                System.exit(1);
            }
            //=================================================================

            Scanner input = new Scanner(System.in);
            int choice = 0;
            while (true) {
                System.out.println();
                System.out.println("Please choose an action where: \n"
                        + "1: to get the position value. \n"
                        + "2: to read MPU values from the arduino serial. \n"
                        + "3: to change the mode in the arduino. \n"
                        + "0: to exit from the system.");
                System.out.print("Please type here: ");
                choice = input.nextInt();

                //=================================================================
                if (choice == 1) {
                    //The R function:
                    re.assign("GyroX", GyroXvalues);
                    re.assign("GyroY", GyroYvalues);
                    re.assign("GyroZ", GyroZvalues);
                    re.assign("Position", Position);
//=================================================================
//                    re.eval("Myobj = lm(Position ~ GyroX + GyroY + GyroZ)");
//                    RVector Myobj = re.eval("Myobj").asVector();
//                    System.out.println("The Intercept is : " + Myobj.at(0).asDoubleArray()[0]);
//                    System.out.println("The GX is : " + Myobj.at(0).asDoubleArray()[1]);
//                    System.out.println("The GY is : " + Myobj.at(0).asDoubleArray()[2]);
//                    System.out.println("The GZ is : " + Myobj.at(0).asDoubleArray()[3]);
//=================================================================
                    re.eval("Gx <- matrix(GyroX,ncol=1,byrow=FALSE)");
                    re.eval("Gy <- matrix(GyroY,ncol=1,byrow=FALSE)");
                    re.eval("Gz <- matrix(GyroZ,ncol=1,byrow=FALSE)");
                    re.eval("colnames(Gx) <- \"GyroX\"");
                    re.eval("colnames(Gy) <- \"GyroY\"");
                    re.eval("colnames(Gz) <- \"GyroZ\"");
                    re.eval("colnames(pos) <- \"Position\"");
                    re.eval("allData = cbind(Gx,Gy,Gz,pos)");
                    re.eval("myDataframe =as.data.frame(allData)");
//                    re.eval("mynum <- myDataframe$GyroX[1]");
//                    double myobj = re.eval("mynum").asDouble();
//                    System.out.println("Value is : " + myobj);
                    re.eval("library(class)");
                    re.eval("traindata = myDataframe[1:3]");
                    re.eval("trainlabels = myDataframe[4]");
                    re.eval("iris_pred <- knn(train = traindata, test = traindata, cl = trainlabels[,1], k=13)");

                } //=================================================================
                else if (choice == 2) {
                    //Read from the arduino serial using the MPU class methods:
                    MPU myMPU = new MPU();
                    String readings = myMPU.readFromSerial();
                    double myGyroXvalue = myMPU.ReadGyroX(readings);
                    double myGyroYvalue = myMPU.ReadGyroY(readings);
                    double myGyroZvalue = myMPU.ReadGyroZ(readings);
                    myMPU.setGyroX(myGyroXvalue);
                    myMPU.setGyroY(myGyroYvalue);
                    myMPU.setGyroZ(myGyroZvalue);

                    System.out.println("My Gyro values are: \n"
                            + " X: " + myMPU.getGyroX() + " and Y: " + myMPU.getGyroY() + " and Z: " + myMPU.getGyroZ());
                    System.out.println();
                } //=================================================================
                else if (choice == 3) {
                    Arduino arduino = new Arduino("COM4", 9600); //enter the port name here, and ensure that Arduino is connected, otherwise exception will be thrown.
                    arduino.openConnection();
                    System.out.println("Enter 1 to switch to Training mode or 2 to Operational mode");
                    char mode = input.nextLine().charAt(0);
                    arduino.serialWrite(mode);
                    arduino.closeConnection();

                } //=================================================================
                else if (choice == 0) {
                    break;
                } else {
                    System.out.println("Please choose a correct number!");
                }
            }
            //close the rengine and the resultset
            re.end();
            rs.close();
            input.close();

        } catch (SQLException e) {
            System.err.println(e);
            System.exit(0);
        } catch (Exception e) {
            System.err.println(e);
            System.exit(0);
        } finally {
            try {
                conn.close();
            } catch (Exception e) {

            }
        }
    }
}
