#include <Detector.hpp>
#include <Camera.hpp>
#include <myUtils.hpp>
#include <ServerAndroid.hpp>

using namespace std;

int main (){
    myUtils::share_print("hellow");
    Detector* detector = new Detector();
    ServerAndroid* server = new ServerAndroid();

    detector->registerCamera(Camera::getInstance());
    server->registerDetector(detector);

    detector->start();
    server->start();

    detector->join();
    server->join();
    
    myUtils::share_print("finished!");
}