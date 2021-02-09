#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <stdlib.h>
#include <myUtils.hpp>
#include <mutex>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#define CAMERA_DEVICE_NUMBER 0

//An simple Wrapper of camera
using namespace cv;

class Camera{

    private:
    VideoCapture cap;
    std::mutex camMtx;


    Camera(){
        myUtils::share_print("open the camera!");
        cap.open(CAMERA_DEVICE_NUMBER);
        if(!cap.isOpened()){
            myUtils::share_print("camera open failed!!!");
            exit(0);
        }
    }

    public:
    //singleton parttern, the connection between application and camera exist only one
    static Camera* getInstance(){
        static Camera instance;
        return &instance;
    }

    //This Method get a frame from camera thread-safely
    Mat getFrame(){
        Mat frame;
        std::lock_guard<std::mutex> gaurd(this->camMtx);
        cap>>frame;
        return frame;
    }

};


#endif