
#include <server_universal.hpp>
#include <server_video.hpp>
#include <module_exception.hpp>
#include <detector.hpp>
#include <lock_control.hpp>
#include <detect_manager.hpp>

#include <python3.7m/Python.h>
#include <pigpio.h>

#include "main_defs.hpp"

using namespace std;
using namespace wayenvan;


int main (){
    Py_Initialize();
    gpioInitialise();

    //start all modules
    LockControl::LockControlPonter lc(new LockControl(LOCKER_SERVO_PIN));
    Detector::DetectorPointer detector(new Detector("/home/pi/Documents/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml"));
    DetectManager::DetectManagerPointer detect_manager(new DetectManager());
    ServerVideo::ServerVideoPointer server_video(new ServerVideo(VIDEO_SERVER_PORT));
    ServerUniversal::ServerUniversalPointer server_uni(new ServerUniversal(UNIVERSAL_SERVER_PORT));


    //register all things 
    detector -> regsisterDetectManager(detect_manager);
    detect_manager -> registerLockControl(lc);
    server_video -> registerDetector(detector);
    server_uni -> registerModules(lc);

    //start all runable
    lc -> start();
    detector -> start();
    detect_manager -> start();
    server_uni -> start();
    server_video -> start();

    //join 
    lc -> join();
    detect_manager -> join();
    detector -> join();
    server_uni ->join();
    server_video -> join();
    

    gpioTerminate();
    Py_Finalize();
    return 0;
}