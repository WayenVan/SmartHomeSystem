package FE;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

/**
 * @author by Liangyue
 * @date 17/04/2021.
 */
public class TempHandler implements Runnable{
    private HomePage homePage;
    private Socket socket;
    private OutputStream os;

    public TempHandler(HomePage homePage) throws IOException {
        this.homePage = homePage;
        // Create a stream socket and connect it to the port and IP address
        this.socket = new Socket("192.168.0.104", 1148);
        // get the output stream
        this.os = socket.getOutputStream();
    }

    @Override
    public void run() {

        while (true){
            try {
                // Temperature
                int intTemp = 0x11;
                intTemp = Integer.reverseBytes(intTemp);
                os.write(ByteBuffer.allocate(4).putInt(intTemp).array());
                getTemp();

                // Humidity
                int intHumi = 0x12;
                intHumi = Integer.reverseBytes(intHumi);
                os.write(ByteBuffer.allocate(4).putInt(intHumi).array());
                getHumi();

                // Pressure
                int intPres = 0x13;
                intPres = Integer.reverseBytes(intPres);
                os.write(ByteBuffer.allocate(4).putInt(intPres).array());
                getPres();
                Thread.sleep(1000);

            }catch (Exception e){
                System.err.println(e);
            }
        }
    }

    private int getLocker() throws IOException {
        // Use socket to read the data returned by the server
        InputStream in = socket.getInputStream();
        byte[] buffer = in.readNBytes(4);

        // Byte to int
        int intLocker = ByteBuffer.wrap(buffer).getInt();
        intLocker = Integer.reverseBytes(intLocker);
        homePage.showLocker(intLocker);
        System.out.println("Locker state: " + intLocker);

        return intLocker;
    }

    private double getTemp() throws IOException {
        // Use socket to read the data returned by the server
        InputStream in = socket.getInputStream();
        byte[] buffer = in.readNBytes(4);

        // Convert byte to double
        int intTemp = ByteBuffer.wrap(buffer).getInt();
        intTemp = Integer.reverseBytes(intTemp);
        double doubleTemp = (double) intTemp/1000.0;
        homePage.showTemp(doubleTemp);

        // Test
        System.out.println("Temperature：" + doubleTemp);

        return doubleTemp;
    }

    private double getHumi() throws IOException {
        // Use socket to read the data returned by the server
        InputStream in = socket.getInputStream();
        byte[] buffer = in.readNBytes(4);

        // Convert byte to double
        int intHumi = ByteBuffer.wrap(buffer).getInt();
        intHumi = Integer.reverseBytes(intHumi);
        double doubleHumi = (double) intHumi /1000.0;
        homePage.showHumi(doubleHumi);

        // Test
        System.out.println("Humidity：" + doubleHumi);

        return doubleHumi;
    }

    private double getPres() throws IOException {
        // Use socket to read the data returned by the server
        InputStream in = socket.getInputStream();
        byte[] buffer = in.readNBytes(4);

        // Convert byte to double
        int intPres = ByteBuffer.wrap(buffer).getInt();
        intPres = Integer.reverseBytes(intPres);
        double doublePres = (double)intPres /1000.0;
        homePage.showPres(doublePres);

        // Test
        System.out.println("Pressure：" + doublePres);

        return doublePres;
    }

}
