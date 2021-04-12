#include<iostream>
#include<amg8833.hpp>
#include<python3.7m/Python.h>

using namespace wayenvan;
using namespace std;

int main(int argc, char** argv){
    Py_Initialize();

    AMG8833* a = AMG8833::getInstance();
    sleep(4);
    while(1){
       cout<<a->getTemperature(4, 4)<<endl;
       sleep(1);
    }
  

    Py_Finalize();
}