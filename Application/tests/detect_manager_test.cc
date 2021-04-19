#include<detect_manager.hpp>
#include<lock_control.hpp>
#include<python3.7m/Python.h>
#include<pigpio.h>
#include<thread>
#include<iostream>

using namespace wayenvan;

int main(){

    gpioInitialise();
    Py_Initialize();

    LockControl::LockControlPonter lock_control(new LockControl(17));
    DetectManager::DetectManagerPointer detect_manager(new DetectManager());

    std::thread T([&](){
        while(1){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout<<detect_manager -> notify()<<std::endl;
        }
    });
    

    detect_manager->registerLockControl(lock_control);

    lock_control -> start();
    detect_manager -> start();

    lock_control -> join();
    detect_manager -> join();
    T.join();

    Py_Finalize();
    gpioTerminate();
    return 0;
}