package hexapod_classifier;

import java.sql.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import org.rosuda.JRI.REXP;
import org.rosuda.JRI.RVector;
import org.rosuda.JRI.Rengine;

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
            int GyroXvalues[] = new int[2600];
            int GyroYvalues[] = new int[2600];
            int GyroZvalues[] = new int[2600];

            //Write the select statment and save it in resultset:
            mySelectStmt = "select * from training_data";
            rs = stmt.executeQuery(mySelectStmt);

            while (rs.next()) {
                GyroXvalues[index] = rs.getInt(2);
                GyroYvalues[index] = rs.getInt(3);
                GyroZvalues[index] = rs.getInt(4);
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
            //for testing purposes:
            int testR = 23;
            re.eval("kkk<-" + testR);
            double i = re.eval("kkk").asDouble();
            System.out.println("");
            System.out.println(i);

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
