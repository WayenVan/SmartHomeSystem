#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

#include <vector>

#include <CppThread.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <camera.hpp>
#include <memory>

namespace wayenvan{

/**
 * @brief detector class is used to detect human face from camera, it has a thread to:
 * - read video frame and do detection
 * - save detected frame into buffer
 * - notify other module if detected 
 * it should register other information */
class Detector: public CppThread{

    typedef boost::circular_buffer<cv::Mat> FrameBuffer;
    typedef std::unique_ptr<FrameBuffer> FrameBufferPointer;

    public:
    typedef std::shared_ptr<Detector> DetectorPointer;
    


    private:
    Camera *camera; 

    //mutex to protect buffer
    std::mutex frame_buffer_mutex_;
    FrameBufferPointer frame_buffer_;

    const int kFrameBufferMax_;

    //frame compress 
    const int kFrameCompressHeight_;
    const int kFrameCompressWidth_;

    //impelement thread run function
    void run();

    public:
    //default constructor
    Detector(const int& max_buffer = 10, const int& frame_compress_height=360, const int& frame_compress_width = 640):
        camera(Camera::getInstance()), 
        frame_buffer_mutex_{}, 
        frame_buffer_(nullptr), 
        kFrameBufferMax_(max_buffer),
        kFrameCompressWidth_(frame_compress_width),
        kFrameCompressHeight_(frame_compress_height)
    {
        frame_buffer_ = FrameBufferPointer(new FrameBuffer(kFrameBufferMax_));
        myUtils::share_print("detector initialized");
    }
    ~Detector(){
    }

    //save frame into frame_buffer_
    void bufferPush(const cv::Mat& frame);
    //get frame from frame_buffer_. return true if success
    bool bufferPop(cv::Mat& frame);
};
}

#endif