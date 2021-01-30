#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdlib.h>
#include <share_print.hpp>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

//An simple Wrapper of camera
using namespace cv;

class Camera{

    private:
    VideoCapture cap;

    Camera(){
        share_print("open the camera!");
        cap.open(0);
        if(!cap.isOpened()){
            share_print("camera open failed!!!");
        }
    }

    public:
    //singleton
    static Camera* getInstance(){
        static Camera instance;
        return &instance;
    }

    //get frame from camera
    Mat getFrame(){
        Mat frame;
        cap>>frame;
        return frame;
    }

};


#endif