#include<server_universal.hpp>

using namespace std;
using namespace wayenvan;

int main(int argc, char** argv){
    ServerUniversal s("1148");

    s.start();
    s.join();

}