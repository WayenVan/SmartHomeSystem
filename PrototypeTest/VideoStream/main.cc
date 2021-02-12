#include <Detector.hpp>
#include <Camera.hpp>
#include <myUtils.hpp>
#include <ServerVideo.hpp>

using namespace std;

int main (){
    try{
        myUtils::share_print("hello");
        Detector* detector = new Detector();
        ServerVideo* server = new ServerVideo();

        detector->registerCamera(Camera::getInstance());
        server->registerDetector(detector);

        detector->start();
        server->start();

        detector->join();
        server->join();
    }catch(const std::exception& e){
        myUtils::share_cerr(e.what());
    }

    myUtils::share_print("finished!");
}