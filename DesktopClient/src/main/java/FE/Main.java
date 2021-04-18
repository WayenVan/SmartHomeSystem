package FE;

import java.io.IOException;

/**
 * @author by Liangyue
 * @date 15/04/2021.
 *
 * 1. Showing face video and detected it.
 * 2. Locker open and off.
 * 3. Showing temperature, humidity and pressure.
 *
 * a. Thread one to control video.
 * b. Thread two to show temperature, humidity, pressure and locker status.
 */
public class Main {

    public static void main(String[] args) throws IOException {

        HomePage homePage = new HomePage();

        VideoHandler videoHandler = new VideoHandler(homePage);
        new Thread(videoHandler).start();

        TempHandler tempHandler = new TempHandler(homePage);
        new Thread(tempHandler).start();

        LockerControl lockerControl = new LockerControl(homePage);
        new Thread(lockerControl).start();
    }

}
