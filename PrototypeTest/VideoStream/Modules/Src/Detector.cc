#include <detector.hpp>
#include <stdlib.h>
#include <thread>
#include <my_utils.hpp>
#include <module_exception.hpp>

namespace wayenvan{

//Detector thread
void Detector::run(){

    //check the camera
    if(camera == NULL){
        ModuleException e(myUtils::get_type(*this), "the camera is not opened correctly");
        throw e;
    }
    cv::Mat frame;
    while(1){
        if(!camera->getFrame(frame)){
            // if the frame return false
            continue;
        }
        //compress image
        cv::resize(frame, frame, cv::Size(kFrameCompressWidth_, kFrameCompressHeight_));
        bufferPush(frame);
        std::this_thread::sleep_for (std::chrono::milliseconds(30));         
    }
}

bool Detector::bufferPop(cv::Mat& frame){
    std::lock_guard<std::mutex> guard(frame_buffer_mutex_);
    //the buffer is full
    if(frame_buffer_->empty()) return false; 
     
    //pop a frame from buffer
    frame = frame_buffer_->front();
    frame_buffer_->pop_front();
    return true;
}

void Detector::bufferPush(const cv::Mat& frame){
    std::lock_guard<std::mutex> guard(frame_buffer_mutex_);
    frame_buffer_->push_back(frame);
}

}






