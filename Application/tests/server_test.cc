#include <camera.hpp>
#include <my_utils.hpp>
#include <server_video.hpp>
#include <memory>
#include <module_exception.hpp>

using namespace std;
using namespace wayenvan;

int main (){

    typedef unique_ptr<ServerVideo> ServerVideo_ptr;

    try{
        
        myUtils::share_print("hello");
        Detector::DetectorPointer detector =Detector::DetectorPointer(new Detector("/home/pi/Documents/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml"));
        ServerVideo_ptr server = ServerVideo_ptr(new ServerVideo("12345"));

        server->registerDetector(detector);

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