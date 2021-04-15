#ifndef _SERVER_VIDEO_H_
#define _SERVER_VIDEO_H_

#include <CppThread.hpp>
#include <detector.hpp>

namespace wayenvan{


class ServerVideo: public CppThread{

    private:
    Detector::DetectorPointer detector;

    //change ip
    const char* kServerPort_;

    void run();
    int createAndBind(const char* port);
    int makeSocketNonBlocking(const int& sfd);

    /**
     * send a frame through a socket
     * @brief the socket should be nonblock
     * @return it return 1 if the socket connection occur some unexpected error
     * else return 0
     */
    int sendFrame(const int socket, cv::Mat& frame);

    
    public:
    ServerVideo(const char* server_port): 
        detector(nullptr), 
        kServerPort_(server_port)
    {
        myUtils::share_print("video server initialized");
        
    }
    
    ~ServerVideo() {}

    void registerDetector(Detector::DetectorPointer detector){
        this->detector = detector;
    }

    
};

}

#endif