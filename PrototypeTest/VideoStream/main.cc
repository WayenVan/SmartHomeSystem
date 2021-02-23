
#include <detector.hpp>
#include <camera.hpp>
#include <my_utils.hpp>
#include <server_video.hpp>
#include <memory>
#include <module_exception.hpp>

using namespace std;
using namespace wayenvan;

int main (){
    typedef unique_ptr<Detector> Detector_ptr;
    typedef unique_ptr<ServerVideo> ServerVideo_ptr;

    try{
        myUtils::share_print("hello");
        Detector_ptr detector =Detector_ptr(new Detector());
        ServerVideo_ptr server = ServerVideo_ptr(new ServerVideo("0.0.0.0", 12345));

        detector->registerCamera(Camera::getInstance());
        server->registerDetector(detector.get());

        detector->start();
        server->start();

        detector->join();
        server->join();
    }catch(const ModuleException& e){
        myUtils::share_cerr("Error ocurr in "+string(e.get_error_module_name())+":");
        myUtils::share_cerr(e.what());
    }

    myUtils::share_print("finished!");
}