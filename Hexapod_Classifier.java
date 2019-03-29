package hexapod_classifier;

import java.sql.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import org.rosuda.JRI.*;

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
            
            //String createTable = "CREATE TABLE coefficients (Intercept INT,GyroX INT, GyroY INT, GyroZ INT, PRIMARY KEY (Intercept))";
            //String insertInto = "INSERT INTO coefficients (Intercept,GyroX, GyroY, GyroZ) values (44523,2343,52334,223)";
            // stmt.execute(createTable);

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
                System.out.println(index + " " + GyroXvalues[index]);
                index++;
            }
            
            
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
            //The lm function:
            re.assign("GyroX", GyroXvalues);
            re.assign("GyroY", GyroYvalues);
            re.assign("GyroZ", GyroZvalues);
            re.assign("Position", Position);
            re.eval("Myobj = lm(Position ~ GyroX + GyroY + GyroZ)");
            RVector Myobj = re.eval("Myobj").asVector();
            System.out.println("The Intercept is : " + Myobj.at(0).asDoubleArray()[0]);
            System.out.println("The GX is : " + Myobj.at(0).asDoubleArray()[1]);
            System.out.println("The  GY is : " + Myobj.at(0).asDoubleArray()[2]);
            System.out.println("The GZ is : " + Myobj.at(0).asDoubleArray()[3]);
            
            
            
            //close the rengine and the resultset
            re.end();
            rs.close();

        } catch (SQLException e) {
            System.err.println(e);
            
        } finally {
            try {
                conn.close();
            } catch (Exception e) {

            }
        }
    }
}
