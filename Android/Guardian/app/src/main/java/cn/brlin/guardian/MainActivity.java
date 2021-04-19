package cn.brlin.guardian;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    //initial widgets
    ImageView iv;
    Button btn;
    TextView mTvTemp;
    TextView mTvHum;
    TextView mTvPress;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // DO NOT MODIFY THIS!!!
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // DO NOT MODIFY THIS!!!


        iv = (ImageView) findViewById(R.id.imageView);
        btn = (Button) findViewById(R.id.button);
        mTvTemp = (TextView) findViewById(R.id.textTemp);
        mTvHum = (TextView) findViewById(R.id.textHum);
        mTvPress = (TextView) findViewById(R.id.textPress);


        //start thread of image video
        Thread imageThread = new Thread(new GetVideoStream());
        imageThread.start();

        Thread indicatorThread = new Thread(new GetIndicator());
        indicatorThread.start();

        Thread lockerThread = new Thread(new GetLocker());
        lockerThread.start();


    }

    public void openLocker(View v) {
        OpenLocker open = new OpenLocker();
        open.execute();
    }


    class GetVideoStream implements Runnable {
        Socket sct;
        //initial output-stream
        OutputStream os;
        //initial input-stream
        InputStream is;
        //initial bitmap
        Bitmap bitmap;

        @Override
        public void run() {
            try {
                //connect
                sct = new Socket("192.168.0.104", 12345);

                //send a mark to server
                int mark = 0x14;
                mark = Integer.reverseBytes(mark);

                while (true) {
                    //output steam
                    os = sct.getOutputStream();
                    os.write(ByteBuffer.allocate(4).putInt(mark).array());
                    Thread.sleep(100);

                    //get input-stream
                    is = sct.getInputStream();

                    //get img size
                    byte[] buffer = new byte[4];
                    is.read(buffer);
                    byte[] intByte = Arrays.copyOfRange(buffer, 0, 4);
                    ByteBuffer wrapped = ByteBuffer.wrap(intByte);
                    int intIMG = wrapped.getInt();
                    //reverse
                    intIMG = Integer.reverseBytes(intIMG);

                    System.out.println(intIMG);

                    if (intIMG > 0) {
                        //get bitmap byte array according to the image size
                        byte[] imgBuffer = getDataFromSocket(is, intIMG);

                        //set bitmap onto image-view widget
                        bitmap = BitmapFactory.decodeByteArray(imgBuffer, 0, imgBuffer.length);
                        //iv.setImageBitmap(bitmap);

                        //change widget through main activity thread
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                iv.setImageBitmap(bitmap);
                            }
                        });
                    }
                }
            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }

        }

        /**
         * Get img buffer from input-stream, known img size.
         *
         * @param ins       input stream
         * @param data_size img size get from input-stream
         * @return img buffer
         * @throws IOException
         */
        private byte[] getDataFromSocket(InputStream ins, int data_size) throws IOException {

            int rec_size = 0;
            byte[] data = new byte[0];
            while (rec_size < data_size) {
                int count = 0;
                byte[] buf = new byte[1024 * 10];

                System.out.println(ins.available());
                count = ins.read(buf);

                if (count == -1) {
                    break;
                }

                byte[] template = new byte[data.length + count];
                System.arraycopy(data, 0, template, 0, data.length);
                System.arraycopy(buf, 0, template, data.length, count);

                data = template;
                rec_size += count;
            }

            return data;
        }
    }

    class GetIndicator implements Runnable {
        Socket sct;

        //initial output stream
        OutputStream os;
        //initial input stream


        @Override
        public void run() {
            try {
                //connect
                sct = new Socket("192.168.0.104", 1148);


                //mark for sending to server
                //represent temperature, humidity, pressure
                int mTemp = 0x11;
                mTemp = Integer.reverseBytes(mTemp);

                int mHumid = 0x12;
                mHumid = Integer.reverseBytes(mHumid);

                int mPress = 0x13;
                mPress = Integer.reverseBytes(mPress);


                while (true) {
                    //Get Temperature
                    //output stream
                    os = sct.getOutputStream();
                    os.write(ByteBuffer.allocate(4).putInt(mTemp).array());
                    double temp = getTemp();
                    String sTemp = String.format("%.2f", temp);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mTvTemp.setText(sTemp + "℃");
                        }
                    });
                    Thread.sleep(1000);

                    //Get Humidity
                    os = sct.getOutputStream();
                    os.write(ByteBuffer.allocate(4).putInt(mHumid).array());
                    double humid = getHumid();
                    String sHumid = String.format("%.2f", humid);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mTvHum.setText(sHumid + "%RH");
                        }
                    });
                    Thread.sleep(1000);

                    //Get Pressure
                    os = sct.getOutputStream();
                    os.write(ByteBuffer.allocate(4).putInt(mPress).array());
                    double pressure = getPressure();
                    String sPressure = String.format("%.2f", pressure);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mTvPress.setText(sPressure + "PA");
                        }
                    });
                    Thread.sleep(1000);


                }

            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }
        }

        /**
         * Get Temperature
         *
         * @return temp
         * @throws IOException
         */
        private double getTemp() throws IOException {
            InputStream is;
            //get Temp data
            is = sct.getInputStream();
            byte[] buffer = new byte[4];
            is.read(buffer);

            // Byte to double
            int intTemp = ByteBuffer.wrap(buffer).getInt();
            intTemp = Integer.reverseBytes(intTemp);
            double doubleTemp = intTemp / 1000.0;

            // Terminal Log
            System.out.println("Temperature: " + doubleTemp);

            return doubleTemp;
        }

        /**
         * Get Humidity
         *
         * @return humid
         * @throws IOException
         */
        private double getHumid() throws IOException {
            InputStream is;
            //get Humid data
            is = sct.getInputStream();
            byte[] buffer = new byte[4];
            is.read(buffer);

            // Byte to double
            int intHumid = ByteBuffer.wrap(buffer).getInt();
            intHumid = Integer.reverseBytes(intHumid);
            double doubleHumid = intHumid / 1000.0;

            // Terminal Log
            System.out.println("Humidity: " + doubleHumid);

            return doubleHumid;
        }

        /**
         * Get Pressure
         *
         * @return pressure
         * @throws IOException
         */
        private double getPressure() throws IOException {
            InputStream is;
            //get Pressure data
            is = sct.getInputStream();
            byte[] buffer = new byte[4];
            is.read(buffer);

            // Byte to double
            int intPressure = ByteBuffer.wrap(buffer).getInt();
            intPressure = Integer.reverseBytes(intPressure);
            double doublePressure = intPressure / 1000.0;

            // Terminal Log
            System.out.println("Pressure: " + doublePressure);

            return doublePressure;
        }



    }


    class GetLocker implements Runnable {
        Socket sct;
        OutputStream os;
        //initial input stream

        //lock flag
        int lockPrv = 0;
        int lockAft = 0;

        @Override
        public void run() {
            try {
                sct = new Socket("192.168.0.104", 1148);

                int mLocker = 0x16;
                mLocker = Integer.reverseBytes(mLocker);


                while (true) {
                    //Get locker status
                    os = sct.getOutputStream();
                    os.write(ByteBuffer.allocate(4).putInt(mLocker).array());
                    getLockerStatus();
                    if (lockPrv == 0 && lockAft == 1) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(getApplicationContext(), "Door open!", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                    Thread.sleep(100);
                }

            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }

        }

        private void getLockerStatus() throws IOException {
            InputStream is;
            //get Locker Status data
            is = sct.getInputStream();
            byte[] buffer = new byte[4];
            is.read(buffer);

            // Byte to double
            int intLockerStatus = ByteBuffer.wrap(buffer).getInt();
            intLockerStatus = Integer.reverseBytes(intLockerStatus);
            intLockerStatus /= 1000;

            //change status
            lockPrv = lockAft;
            lockAft = intLockerStatus;

            // Terminal Log
            System.out.println("Locker Status: " + intLockerStatus);
        }
    }

}