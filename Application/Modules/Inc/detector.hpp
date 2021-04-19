/* 
 * This file is part of the SmartHomeSystem distribution (https://github.com/WayenVan/SmartHomeSystem).
 * Copyright (c) 2021 Jingyan Wang.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

#include <vector>

#include <CppThread.hpp>
#include <boost/circular_buffer.hpp>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <camera.hpp>
#include <module_exception.hpp>
#include <memory>
#include <detect_manager.hpp>


namespace wayenvan{


namespace {
    const int kScale = 3.0;
}
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
    DetectManager::DetectManagerPointer detect_manager_;

    //mutex to protect buffer
    std::mutex frame_buffer_mutex_;
    FrameBufferPointer frame_buffer_;

    const int kFrameBufferMax_;

    //frame compress size 
    std::mutex compress_size_mutex_; 
    int frame_compress_height_;
    int frame_compress_Width_;

    //cascade 
    cv::CascadeClassifier face_cascade_;

    //impelement thread run function
    void run();

    public:
    //default constructor
    Detector(const std::string cascade_dir,
            const int& max_buffer = 10, 
            const int& frame_compress_height=360, 
            const int& frame_compress_width = 640):
        camera(Camera::getInstance()),
        detect_manager_(), 
        frame_buffer_mutex_{}, 
        frame_buffer_(nullptr), 
        kFrameBufferMax_(max_buffer),
        frame_compress_Width_(frame_compress_width),
        frame_compress_height_(frame_compress_height),
        face_cascade_()
    {
        if(!face_cascade_.load(cascade_dir)){
            ModuleException e(myUtils::get_type(this), "cascade file is not poaded properly");
            throw e;
        }

        frame_buffer_ = FrameBufferPointer(new FrameBuffer(kFrameBufferMax_));
        myUtils::share_print("detector initialized");
    }
    ~Detector(){
    }

    void regsisterDetectManager(DetectManager::DetectManagerPointer ins){
        detect_manager_ = ins;
        if(detect_manager_.get() == nullptr){
            ModuleException e(myUtils::get_type(this), "detect manager is register failed");
            throw e;
        }
    }

    void setCompressSize(const int height, const int width){
        std::lock_guard<std::mutex> guard(compress_size_mutex_);
        frame_compress_height_ = height;
        frame_compress_Width_ = width;
    }

    //save frame into frame_buffer_
    void bufferPush(const cv::Mat& frame);
    //get frame from frame_buffer_. return true if success
    bool bufferPop(cv::Mat& frame);
};
}

#endif