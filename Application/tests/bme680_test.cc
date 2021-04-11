#include<python2.7/Python.h>
#include<iostream>
#include<bme680.hpp>

using namespace wayenvan;
using namespace std;

int main(){
    Py_Initialize();
    
    Bme680* b = Bme680::getInstance(); 
    int i = 0;
    while(i<20){
        cout<<b->getTemperature()<<endl;
        cout<<b->getPressure()<<endl;
        cout<<b->getHumidity()<<endl;
        sleep(1);
        i++;
    }
    Py_Finalize();
    return 0;
}