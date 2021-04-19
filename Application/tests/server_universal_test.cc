#include<server_universal.hpp>
#include<python3.7m/Python.h>

using namespace std;
using namespace wayenvan;

int main(int argc, char** argv){

    Py_Initialize();
    
    ServerUniversal s("1148");

    s.start();
    s.join();

}