#include <Detector.hpp>
#include <Camera.hpp>
#include <myUtils.hpp>
#include <ServerVideo.hpp>

using namespace std;

int main (){
    myUtils::share_print("hello");
    Detector* detector = new Detector();
    ServerVideo* server = new ServerVideo();

    detector->registerCamera(Camera::getInstance());
    server->registerDetector(detector);

    detector->start();
    server->start();

    detector->join();
    server->join();
    
    delete detector;
    delete server;

    myUtils::share_print("finished!");
}