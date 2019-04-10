package Smart_Classifier;

import java.sql.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import org.rosuda.JRI.*;
import arduino.*;

public class Smart_Classifier {

    //Use the following variable to connect to the DB:
    private static final String USERNAME = "root";
    private static final String PASSWORD = "";
    private static final String CONN_STRING = "jdbc:mysql://127.0.0.1/hexapod_cf";

    public static void main(String[] args) {
        Connection conn = null;
        try {
            //Create connection to the local DB and print a message:
            conn = DriverManager.getConnection(CONN_STRING, USERNAME, PASSWORD);
            System.out.println("Connection to DB is successful");

            //Create a statment and a resultset and an MPU instance:
            Statement stmt = conn.createStatement();
            ResultSet rs = null;
            MPU myMPU = new MPU();

            //=================================================================
            //String createTable = "CREATE TABLE coefficients (Intercept INT,GyroX INT, GyroY INT, GyroZ INT, PRIMARY KEY (Intercept))";
            //String insertInto = "INSERT INTO coefficients (Intercept,GyroX, GyroY, GyroZ) values (44523,2343,52334,223)";
            //stmt.execute(createTable);
            //=================================================================
            //Identify variables:
            String mySelectStmt;
            int index = 0;
            //Create arrays to store the dataset made in the training session from the DB:
            double GyroXvalues[] = new double[2600];
            double GyroYvalues[] = new double[2600];
            double GyroZvalues[] = new double[2600];
            double Position[] = new double[2600];
            double pos;
            String message;
            char oneChar = 0;
            char mode = 0;
            int myPos;
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
            //Use a scanner to read inputs from the user:
            Scanner input = new Scanner(System.in);
            int choice = 0;
            while (true) {
                try {
                    System.out.println();
                    System.out.println("Please choose an action where: \n"
                            + "1: to read MPU values from the arduino serial. \n"
                            + "2: to get the position value.\n"
                            + "3: to change the mode in the arduino. \n"
                            + "0: to exit from the system.");
                    System.out.print("Please type here: ");
                    choice = input.nextInt();

                    //=================================================================
                    if (choice == 1) {
                        //Read from the arduino serial using the MPU class methods:
                        System.out.println("Reading from Serial...");
                        oneChar = 'R';

                        String readings = myMPU.readFromSerial(oneChar);    //To send it to the serial monitor at the arduino IDE
                        //After getting the readings from the serial use the reading to get the values using the read methods (Explained in the MPU class):
                        double myGyroXvalue = myMPU.ReadGyroX(readings);
                        double myGyroYvalue = myMPU.ReadGyroY(readings);
                        double myGyroZvalue = myMPU.ReadGyroZ(readings);
                        //Set it into myMPU object created above:
                        myMPU.setGyroX(myGyroXvalue);
                        myMPU.setGyroY(myGyroYvalue);
                        myMPU.setGyroZ(myGyroZvalue);

                        System.out.println("My Gyro values are: \n"
                                + " X: " + myMPU.getGyroX() + " and Y: " + myMPU.getGyroY() + " and Z: " + myMPU.getGyroZ());
                        System.out.println();
                    } //=================================================================
                    else if (choice == 2) {
                        //After getting the current Gyro values we can classify the postion using R commands:
                        //First we identified the current gyro values into variables in R:
                        System.out.println(re.eval("gX <- " + myMPU.getGyroX()));
                        System.out.println(re.eval("gY <- " + myMPU.getGyroY()));
                        System.out.println(re.eval("gZ <- " + myMPU.getGyroZ()));
                        //Then we assign the arrays which we stored the training dataset in it into a named variables:
                        re.assign("GyroX", GyroXvalues);
                        re.assign("GyroY", GyroYvalues);
                        re.assign("GyroZ", GyroZvalues);
                        re.assign("Position", Position);
                        //then we make matrixes out of the arrays:
                        System.out.println(re.eval("Gx <- matrix(GyroX,ncol=1,byrow=FALSE)"));
                        System.out.println(re.eval("Gy <- matrix(GyroY,ncol=1,byrow=FALSE)"));
                        System.out.println(re.eval("Gz <- matrix(GyroZ,ncol=1,byrow=FALSE)"));
                        System.out.println(re.eval("pos <- matrix(Position,ncol=1,byrow=FALSE)"));
                        //Name the column of each matrix:
                        System.out.println(re.eval("colnames(Gx) <- \"GyroX\""));
                        System.out.println(re.eval("colnames(Gy) <- \"GyroY\""));
                        System.out.println(re.eval("colnames(Gz) <- \"GyroZ\""));
                        System.out.println(re.eval("colnames(pos) <- \"Position\""));
                        //Combine all matrixes into one matrix:
                        System.out.println(re.eval("allData = cbind(Gx,Gy,Gz,pos)"));
                        //And then make the dataframe from the combined matrix (all steps above were made to make the following dataframe):
                        System.out.println(re.eval("myDataframe =as.data.frame(allData)"));

                        //re.eval("install.packages(\"class\")");
                        System.out.println(re.eval("library(class)"));
                        //Now that we have our training dataset we can use the Knn algorithm:
                        //Save the first three columns (which are the Gyro X, Y, and Z) in a seperate dataframe:
                        System.out.println(re.eval("traindata = myDataframe[1:3]"));
                        //And the position column in a seperate datafram:
                        System.out.println(re.eval("trainlabels = myDataframe[4]"));
                        //The following kNN function is to view the accuracy and the result of using the kNN function with our data:
                        System.out.println(re.eval("myknn <- knn(train = traindata, test = traindata, cl = trainlabels[,1], k=13)"));

                        //re.eval("install.packages(\"caret\")");
                        System.out.println(re.eval("library(caret)"));
                        //In the following method we train our data using the kNN method:
                        System.out.println(re.eval("model_knn <- train(traindata, trainlabels[,1], method='knn')"));
                        //The predict method will determine the position based on the training we did:
                        System.out.println(re.eval("predictions<-predict(object=model_knn,traindata)"));
                        //Save the current Gyro values we got earlier into a dataframe and name it x:
                        System.out.println(re.eval("x <- data.frame(\"GyroX\" =gX, \"GyroY\" =gY,\"GyroZ\" = gZ)"));
                        //Save the position we got out of the x dataframe and save it to y:
                        System.out.println(re.eval("y <- predict(object=model_knn,x)"));

                        //Get y as a double value:
                        pos = re.eval("y").asDouble();
                        //Cast it into an integer and print it:
                        myPos = (int) pos;
                        System.out.println("Value is : " + myPos);

                        //print the following in the arduino serial by making an Arduino object and serialWrite method:
                        message = "" + myPos;
                        char one = 0;
                        //Choose the Port number and the serail baud rate:
                        Arduino arduino = new Arduino("COM4", 9600); //enter the port name here, and ensure that Arduino is connected, otherwise exception will be thrown.
                        arduino.openConnection();

                        if (message.length() >= 2) {
                            arduino.serialWrite(message, message.length(), 1000);

                        } else {
                            one = message.charAt(0);
                            arduino.serialWrite(one, 5000);
                        }

                        //Then read the message from the arduino serial which will return the name of the position:
                        message = arduino.serialRead();
                        System.out.println(message);

                        arduino.closeConnection();
                    } //=================================================================
                    else if (choice == 3) {
                        //In this choice the user will determine if he wishes to switch to training mode or go back to operational mode:
                        //Same as above we create arduino object and write into the serial then read the output from the serial:
                        input.nextLine();
                        Arduino arduino = new Arduino("COM4", 9600); //enter the port name here, and ensure that Arduino is connected, otherwise exception will be thrown.
                        arduino.openConnection();
                        System.out.println("Enter (T) to switch to Training mode or (O) to Operational mode");
                        System.out.print("Type the mode you desire: ");
                        mode = input.nextLine().charAt(0);
                        System.out.println("Changing Mode...");
                        arduino.serialWrite(mode, 5000);
                        message = arduino.serialRead();

                        arduino.closeConnection();

                        System.out.println(message);

                    } //=================================================================
                    else if (choice == 0) {
                        //To exit from the system:
                        break;
                    } else {
                        System.out.println("Please choose a correct number!");
                    }
                } catch (InputMismatchException e) {
                    System.err.println(e);
                    System.out.println("Please choose a valid choice!");
                    input.nextLine();
                } catch (StringIndexOutOfBoundsException e) {
                    System.err.println(e);
                    input.nextLine();
                } catch (IndexOutOfBoundsException e) {
                    System.err.println(e);
                    System.out.println("Reading from Gyro faild...");
                    System.out.println("Try again");
                    input.nextLine();

                } catch (Exception e) {
                    System.err.println(e);
                    input.nextLine();
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
