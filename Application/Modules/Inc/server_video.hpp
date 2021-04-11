#ifndef _SERVER_VIDEO_H_
#define _SERVER_VIDEO_H_

#include <CppThread.hpp>
#include <detector.hpp>

namespace wayenvan{


class ServerVideo: public CppThread{

    private:
    Detector::DetectorPointer detector;

    //change ip
    const char* kServerIp_;
    const int kServerPort_;

    void run();
    
    public:
    ServerVideo(const char* server_ip, const int server_port): 
        detector(nullptr), 
        kServerIp_(server_ip), 
        kServerPort_(server_port)
    {
        myUtils::share_print("video server initialized");
        
    }
    
    ~ServerVideo() {}

    void registerDetector(Detector::DetectorPointer detector){
        this->detector = detector;
    }

    //method to handle instruction
    
};

}

#endif