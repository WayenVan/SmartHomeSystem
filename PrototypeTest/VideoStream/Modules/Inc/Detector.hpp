#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

#include <vector>

#include <CppThread.h>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <Camera.hpp>

#define FRAME_BUFFER_MAX 200

class Detector: public CppThread{

    typedef boost::circular_buffer<cv::Mat> MatBuffer;

    private:
    Camera *camera; 

    //mutex to protect buffer
    std::mutex bufferMtx;
    MatBuffer* buffer;

    //impelement thread run function
    void run();

    public:
    //default constructor
    Detector(){
        this->buffer = new MatBuffer(FRAME_BUFFER_MAX);
    }
    ~Detector(){
        delete this->buffer;
    }

    //need to regsister 
    void registerCamera(Camera* camera){
        this->camera = camera;
    }

    //save frame into buffer
    void bufferPush(cv::Mat& frame);
    //get frame from buffer. return true if success
    bool bufferPop(cv::Mat& frame);
};

#endif