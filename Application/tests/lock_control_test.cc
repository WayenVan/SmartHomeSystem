#include<lock_control.hpp> 
#include<thread>
#include<iostream>
#include<pigpio.h>

using namespace std;
using namespace wayenvan;

int main(int argv, char** argc){

    gpioInitialise();

    LockControl lc(17);

    thread t1([&](){
        while(1){
        this_thread::sleep_for(chrono::seconds(3));
        cout<<"t1: " + to_string(lc.notify())<<endl;
        }
    });


    thread t2([&](){
        while(1){
        this_thread::sleep_for(chrono::seconds(8));
        cout<<"t2: " + to_string(lc.notify())<<endl;
        }
    });

    lc.start();

    lc.join();
    t1.join();
    t2.join();

    gpioTerminate();
}