#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

#include <vector>

#include <CppThread.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <camera.hpp>

namespace wayenvan{


class Detector: public CppThread{

    typedef boost::circular_buffer<cv::Mat> FrameBuffer;

    private:
    Camera *camera; 

    //mutex to protect buffer
    std::mutex frame_buffer_mutex_;
    FrameBuffer* frame_buffer_;

    const int kFrameBufferMax_;

    //frame compress 
    const int kFrameCompressHeight_;
    const int kFrameCompressWidth_;

    //impelement thread run function
    void run();

    public:
    //default constructor
    Detector(const int& max_buffer = 20, const int& frame_compress_height=360, const int& frame_compress_width = 640):
        camera(nullptr), 
        frame_buffer_mutex_{}, 
        frame_buffer_(nullptr), 
        kFrameBufferMax_(max_buffer),
        kFrameCompressWidth_(frame_compress_width),
        kFrameCompressHeight_(frame_compress_height)
    {
        this->frame_buffer_ = new FrameBuffer(kFrameBufferMax_);
    }
    ~Detector(){
        delete this->frame_buffer_;
    }

    //need to regsister 
    void registerCamera(Camera* camera){
        this->camera = camera;
    }

    //save frame into frame_buffer_
    void bufferPush(const cv::Mat& frame);
    //get frame from frame_buffer_. return true if success
    bool bufferPop(cv::Mat& frame);
};
}

#endif