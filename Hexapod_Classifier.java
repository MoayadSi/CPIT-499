package hexapod_classifier;

import java.sql.*;

public class Hexapod_Classifier {

    private static final String USERNAME = "root";
    private static final String PASSWORD = "";
    private static final String CONN_STRING = "jdbc:mysql://127.0.0.1/Hexapod_CF";

    public static void main(String[] args) {
        Connection conn = null;
        try {
            conn = DriverManager.getConnection(CONN_STRING, USERNAME, PASSWORD);
            System.out.println("Connecttion to DB is successful");
            Statement stmt = conn.createStatement();

             String createTable = "CREATE TABLE coefficients (Intercept INT,GyroX INT, GyroY INT, GyroZ INT, PRIMARY KEY (Intercept))";
            //String insertInto = "INSERT INTO coefficients (Intercept,GyroX, GyroY, GyroZ) values (44523,2343,52334,223)";

            stmt.execute(createTable);
            conn.close();
        } catch (SQLException e) {
            System.err.println(e);
        }
    }
}
