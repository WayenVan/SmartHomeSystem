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

#include <detector.hpp>
#include <stdlib.h>
#include <thread>
#include <my_utils.hpp>
#include <module_exception.hpp>
#include <algorithm.hpp>
#include <vector>
#include <mutex>

namespace wayenvan{

//Detector thread
void Detector::run(){

    //check the camera
    if(camera == NULL){
        ModuleException e(myUtils::get_type(*this), "the camera is not opened correctly");
        throw e;
    }
    double scale = 3.0;
    cv::Mat frame;
    std::vector<cv::Rect> faces;
    std::unique_lock<std::mutex> locker(compress_size_mutex_);
    locker.unlock();

    while(1){
        //clean faces
        faces.clear();

        if(!camera->getFrame(frame)){
            // if the frame return false
            continue;
        }

        //compress image
        locker.lock();
        cv::resize(frame, frame, cv::Size(frame_compress_Width_, frame_compress_height_));
        locker.unlock();
        
        //face detection method
        algorithm::faceDetect(frame, face_cascade_, scale, faces);

        //face detected
        if(faces.size() > 0) {
            myUtils::share_print("face detected");
            algorithm::drawFaces(frame, faces, scale);
            detect_manager_->notify();
        }
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






