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

#ifndef _DETECT_MANAGER_HPP_
#define _DETECT_MANAGER_HPP_

#include<memory>
#include<mutex>
#include<condition_variable>
#include<thread>

#include<amg8833.hpp>
#include<lock_control.hpp>

#define HUMAN_TEMPERATURE_LOW 30
#define HUMAN_TEMPERATURE_HIGH 34

namespace wayenvan{

/**
 * this class is used to measure temperature and open the door
 * it should be notified by detector module if human face detected
 */
class DetectManager: public CppThread{

    public:
    typedef enum State{
        READY = 0,
        START,
        HANDLING,
    }State;

    typedef std::shared_ptr<DetectManager> DetectManagerPointer;

    private:
    LockControl::LockControlPonter locker_;
    AMG8833* sensor_;

    std::mutex state_mutex_;
    std::condition_variable cv_;
    State state_;

    void run(){
        std::unique_lock<std::mutex> lock(state_mutex_);
        
        while(1){
            if(state_ != START) cv_.wait(lock, [&](){ return state_ == START; });
            state_ = HANDLING;
            lock.unlock();

            //todo opencv lock
            myUtils::share_print("start measure temperature");
            for(int i = 0; i < 5; i++){
                if(sensor_->getTemperature(4, 4) >= 30.0){
                    myUtils::share_print("temperature pass!");
                    locker_->notify();
                    break;
                }

                //sleep 700 ms
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
            }

            //judge if the 

            lock.lock();
            state_ = READY;
        }
    }

    public:

    /**
     * constructor
     */
    DetectManager():
        locker_(nullptr),
        sensor_(AMG8833::getInstance()),
        state_(READY)
    {   
        if(sensor_ == nullptr){
            ModuleException e(myUtils::get_type(this), "amg8833 sensor is null");
            throw e;
        }

        myUtils::share_print("Detect manager initialized");

    }
    ~DetectManager(){}

    //registerLockControl
    void registerLockControl(LockControl::LockControlPonter locker){
        locker_ = locker;
        if(locker_.get() == nullptr){
            ModuleException e(myUtils::get_type(this), "locker control is register failed");
            throw e;
        }
    }

    bool notify(){
        std::lock_guard<std::mutex> lg(state_mutex_);
        
        if(state_ != READY){
            return false;
        }
        state_ = START;
        cv_.notify_all();

        return true;
    }

    State getState(){
        return state_;
    }
    

};

}

#endif