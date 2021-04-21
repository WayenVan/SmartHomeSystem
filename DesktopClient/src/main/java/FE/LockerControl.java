package FE;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

/**
 * @author by Liangyue
 * @date 18/04/2021.
 */
public class LockerControl implements Runnable{

    private  HomePage homePage;
    private Socket socket;
    private OutputStream os;
    private static int lastStatus = 0;

    public LockerControl(HomePage homePage) throws IOException {
        this.homePage = homePage;
        // Create a stream socket and connect it to the port and IP address
        this.socket = new Socket("192.168.0.100", 1148);
        // get the output stream
        this.os = socket.getOutputStream();
    }

    @Override
    public void run() {
        while (true){
            try {
                // Locker state
                int intLocker = 0x16;
                intLocker = Integer.reverseBytes(intLocker);
                os.write(ByteBuffer.allocate(4).putInt(intLocker).array());
                int intStatus = getLocker();
                setStatus(intStatus);
                Thread.sleep(100);

            }catch (Exception e){
                System.err.println(e);
            }
        }
    }

    private int setStatus(int intStatus) {
        lastStatus = intStatus;
        homePage.getLastStatus(lastStatus);
        return lastStatus;
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


}
