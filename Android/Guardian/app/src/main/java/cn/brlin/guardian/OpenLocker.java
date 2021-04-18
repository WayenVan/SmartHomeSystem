package cn.brlin.guardian;

import android.os.AsyncTask;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

/**
 * Open a Asynchronous thread which open locker a time
 */
public class OpenLocker extends AsyncTask<Void, Void, Void> {
    //initial a Socket
    Socket sct;

    @Override
    protected Void doInBackground(Void... voids) {
        try {
            //connect
            sct = new Socket("192.168.0.104", 1148);

            //send a mark and reverse
            int mark = 0x15;
            mark = Integer.reverseBytes(mark);

            //send to server
            OutputStream os = sct.getOutputStream();
            os.write(ByteBuffer.allocate(4).putInt(mark).array());


        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }
}
