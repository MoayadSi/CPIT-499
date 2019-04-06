/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hexapod_classifier;

import javafx.beans.binding.Bindings;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;
import static org.hamcrest.MatcherAssert.assertThat;

public class MPUTest {

    public MPUTest() {
    }

    @BeforeClass
    public static void setUpClass() {
    }

    @AfterClass
    public static void tearDownClass() {
    }

    @Before
    public void setUp() {
    }

    @After
    public void tearDown() {
    }

    /**
     * Test of readFromSerial method, of class MPU.
     */
    @Test
    public void testReadFromSerial() {
        System.out.println("readFromSerial");
        MPU instance = new MPU();
        char c = 0;
        int expResult = 40;
        String result = instance.readFromSerial(c);
        assertEquals(expResult, result.length());
    }

    /**
     * Test of ReadGyroX method, of class MPU.
     */
    @Test
    public void testReadGyroX() {
        System.out.println("ReadGyroX");
        String readings = "GyroX65500.00GyroY65522.00GyroZ65509.00";
        MPU instance = new MPU();
        double expResult = 60000;
        double expResult2 = 69000;
        double result = instance.ReadGyroX(readings);
        assertTrue(result > expResult && result < expResult2);
    }

    /**
     * Test of ReadGyroY method, of class MPU.
     */
    @Test
    public void testReadGyroY() {
        System.out.println("ReadGyroY");
        String readings = "GyroX65500.00GyroY65522.00GyroZ65509.00";
        MPU instance = new MPU();
        double expResult = 60000;
        double expResult2 = 69000;
        double result = instance.ReadGyroY(readings);
        assertTrue(result > expResult && result < expResult2);

    }

    /**
     * Test of ReadGyroZ method, of class MPU.
     */
    @Test
    public void testReadGyroZ() {
        System.out.println("ReadGyroZ");
        String readings = "GyroX65500.00GyroY65522.00GyroZ65509.00";
        MPU instance = new MPU();
        double expResult = 60000;
        double expResult2 = 69000;
        double result = instance.ReadGyroZ(readings);
        assertTrue(result > expResult && result < expResult2);

    }

}
