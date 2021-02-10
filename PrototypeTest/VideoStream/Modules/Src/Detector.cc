#include <Detector.hpp>
#include <stdlib.h>
#include <thread>
#include <myUtils.hpp>

//Detector thread
void Detector::run(){

    //check the camera
    if(this->camera == NULL){
        myUtils::share_cerr("the camera in detector did not register properly");
        exit(0);
    }
    cv::Mat frame;
    while(1){
        frame = this->camera->getFrame();
        cv::resize(frame, frame, cv::Size(640, 360));
        
        this->bufferPush(frame);

        std::this_thread::sleep_for (std::chrono::milliseconds(30));         
    }
}

bool Detector::bufferPop(cv::Mat& frame){
    std::lock_guard<std::mutex> guard(this->bufferMtx);
    //the buffer is full
    if(this->buffer->empty()) return false; 
     
    //pop a frame from buffer
    frame = this->buffer->front();
    this->buffer->pop_front();
    return true;
}

void Detector::bufferPush(cv::Mat& frame){
    std::lock_guard<std::mutex> guard(this->bufferMtx);
    this->buffer->push_back(frame);
}







