package cn.brlink.client;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    Socket sct;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        if (android.os.Build.VERSION.SDK_INT > 9) {
//            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
//            StrictMode.setThreadPolicy(policy);
//        }
//
//        try {
//            sct = new Socket("100.76.133.73", 12345);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//        // start thread contains runnable GetMsgStream
//        Thread thread = new Thread(new GetMsgStream());
//        thread.start();

    }

//    class GetMsgStream implements Runnable {
//        // variables for output
//        PrintWriter pw;
//
//        // variables for input
//        InputStream is;
//        BufferedReader br;
//
//        String msg;
//
//        Handler handler = new Handler();
//
//        @Override
//        public void run() {
//            try {
//                // send message
//                pw = new PrintWriter(sct.getOutputStream());
//                // write message
//                pw.write("Hello, Raspberry PI!");
//                // flush socket
//                pw.flush();
//
//                // receive message
//                is = sct.getInputStream();
//
//                handler.post(new Runnable() {
//                    @Override
//                    public void run() {
//                        byte[] bt = new byte[4];
//                        int rc = 0;
//                        try {
//                            rc = is.read(bt, 0, 4);
//                            Log.v("rc", String.valueOf(rc));
//                            Log.v("bt", String.valueOf(bt[3] & 0xFF |
//                                    (bt[2] & 0xFF) << 8 |
//                                    (bt[1] & 0xFF) << 16 |
//                                    (bt[0] & 0xFF) << 24));
//                        } catch (IOException e) {
//                            e.printStackTrace();
//                        }
//
//                    }
//                });
//
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//
//        }
//    }
}