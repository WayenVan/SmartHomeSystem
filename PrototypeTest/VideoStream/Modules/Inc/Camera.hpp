#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <stdlib.h>
#include <my_utils.hpp>
#include <mutex>
#include <boost/format.hpp>
#include <stdint.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>


//An simple Wrapper of camera


namespace wayenvan{

class Camera{

    typedef cv::Mat Mat;
    typedef cv::VideoCapture VideoCapture;

    private:
    VideoCapture video_capture_;
    std::mutex camera_mutex_;

    const int kCameraDeviceNumber_;

    Camera(const int& kCameraDeviceNumber = 0):
        video_capture_{},
        camera_mutex_{},
        kCameraDeviceNumber_(kCameraDeviceNumber)
    {
        myUtils::share_print("opening the camera...");
        video_capture_.open(kCameraDeviceNumber_);
        if(!video_capture_.isOpened()){
            myUtils::share_print("camera open failed!!!");
            exit(0);
        }else{
            myUtils::share_print("camera open success");

        }

        int width = static_cast<int>(this->video_capture_.get(cv::CAP_PROP_FRAME_WIDTH));
        int length = static_cast<int>(this->video_capture_.get(cv::CAP_PROP_FRAME_HEIGHT));
        //print camera information
        boost::format info("camera resolution: width %1%, height %2%");
        info % width;
        info % length;

        myUtils::share_print(info.str());

    }
    ~Camera(){
        video_capture_.release();
    }

    public:
    //singleton parttern, the connection between application and camera exist only one
    static Camera* getInstance(){
        static Camera instance;
        return &instance;
    }

    //This Method get a frame from camera thread-safely
    bool getFrame(Mat& output){
        Mat frame;
        std::lock_guard<std::mutex> gaurd(this->camera_mutex_);
        video_capture_>>frame;
        if(frame.empty())
        {
            return false;
        }
        frame.copyTo(output);
        return true;
    }

};

}

#endif