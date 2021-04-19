#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <stdlib.h>
#include <my_utils.hpp>
#include <mutex>
#include <boost/format.hpp>
#include <stdint.h>
#include <module_exception.hpp>
#include <typeinfo>
#include <memory>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 360


namespace wayenvan{
    
/** 
 * @brief a simple wrapper of camera, can be used to get frame */
class Camera{

    typedef cv::Mat Mat;
    typedef cv::VideoCapture VideoCapture;

    private:
    VideoCapture video_capture_;
    std::mutex camera_mutex_;

    const int kCameraDeviceNumber_;

    /** 
     * @brief the default constructor of the camera
     * @param kCameraDeviceNumber is the device number of your camera
     * which can be found in /dev/video[x] */
    Camera(const int& kCameraDeviceNumber = 0):
        video_capture_{},
        camera_mutex_{},
        kCameraDeviceNumber_(kCameraDeviceNumber)
    {
        myUtils::share_print("opening the camera...");

        video_capture_.open(kCameraDeviceNumber_);
        if(!video_capture_.isOpened()){
            ModuleException e(myUtils::get_type(*this), "Can not open camera");
            throw e;       
        }
        myUtils::share_print("camera open success");
        std::cout<<cv::getBuildInformation();
        video_capture_.set(cv::CAP_PROP_AUTO_EXPOSURE,0.25);
        video_capture_.set(cv::CAP_PROP_EXPOSURE, 0.9);

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

    /**
     * @brief using singleton mode, always return to the same instance */
    static Camera* getInstance(){
        static Camera instance;
        return &instance;
    }

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