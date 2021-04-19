package FE;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.util.Arrays;

/**
 * @author by Liangyue
 * @date 17/04/2021.
 */
public class VideoHandler implements Runnable{

        private HomePage homePage;
        private Socket socket;

        public VideoHandler(HomePage homePage) throws IOException {
            this.homePage = homePage;
            this.socket = new Socket("192.168.0.104",12345);
            this.socket.setSoTimeout(500);
        }

    @Override
        public void run() {
            try {
                boolean tag = true;
                while (tag){
                    // Obtain the output stream and send data to the server
                    int a = 0x14;
                    a = Integer.reverseBytes(a);
                    OutputStream outputStream = socket.getOutputStream();
                    outputStream.write(ByteBuffer.allocate(4).putInt(a).array());
                    Thread.sleep(100);

                    // Use socket to read the data returned by the server
                    InputStream inputStream = socket.getInputStream();
                    byte[] buffer = inputStream.readNBytes(4);

                    // Convert byte to int
                    byte[] intByte = Arrays.copyOfRange(buffer, 0, 4);
                    ByteBuffer wrapped = ByteBuffer.wrap(intByte);
                    int intIMG = wrapped.getInt();
                    intIMG = Integer.reverseBytes(intIMG);

                    System.out.println(intIMG);
                    if(intIMG > 0){

                        byte[] imgBuffer = getDataFromSocket(inputStream, intIMG);
                        // Convert byte to image
                        //reverse(imgBuffer);
                        homePage.showImage(byteToImage(imgBuffer));
                    }
                }

            }catch (Exception e){
                System.err.println(e);
            }
        }

        private BufferedImage byteToImage(byte[] imgBuffer) throws IOException {
            InputStream is = new ByteArrayInputStream(imgBuffer);
            BufferedImage bufferedImage = ImageIO.read(is);

            return bufferedImage;
        }

        static public byte[] getDataFromSocket(InputStream ins, int data_size) throws IOException {
            int recv_size = 0;
            byte[] data = new byte[0];
            while(recv_size < data_size)
            {
                int count = 0;
                byte[] buf = new byte[1024*10];

                count = ins.read(buf);

                if(count == -1) {
                    break;
                }

                byte[] template = new byte[data.length + count];
                System.arraycopy(data, 0, template, 0, data.length);
                System.arraycopy(buf, 0, template, data.length, count);

                data = template;
                recv_size += count;
            }

            return data;
    }

    static public void sendToSocket(OutputStream outputStream, int data, int size) throws IOException {
            int sent_size = 0;
            while(sent_size < size){
                int count = 0;
                outputStream.write(ByteBuffer.allocate(size).putInt(data).array());
            }
    }

}
