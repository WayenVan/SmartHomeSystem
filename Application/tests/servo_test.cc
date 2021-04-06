
#include<servo.hpp>
#include<pigpio.h>
#include<softPwm.h>
#include<module_exception.hpp>
#include<iostream>
#include<cassert>
#include<unistd.h>
#include<thread>
#include<chrono>

using namespace wayenvan;
using namespace std;

const int port = 17;

int main(int argv, char** argc){


    // bool result1 = s2.reposition(-1);
    // assert(result1 == false);
    // bool result2 = s2.reposition(181);
    // assert(result2 == false);

    //test pigpio
    gpioInitialise();
    gpioSetMode(port, PI_OUTPUT);
    gpioSetPWMfrequency(port, 50);
    gpioSetPWMrange(port, 20000);
    gpioPWM(port, 1500);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    gpioPWM(port, 2500);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));

    //test  servo class

    //test multi thread process
    thread t1([&](){
          //test  servo class
        Servo s(17);
        s.reposition(45);
        s.reposition(90); 

    });

    thread t2([&](){
        Servo s(27);
        s.reposition(45);
        s.reposition(180);
    });

    t1.join();
    t2.join();
    

    gpioTerminate();
    
    return 1;
}

