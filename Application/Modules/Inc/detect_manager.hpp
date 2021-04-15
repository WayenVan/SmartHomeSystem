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



            lock.lock();
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
        
    }
    ~DetectManager(){}

    //registerLockControl
    void registerLockControl(LockControl::LockControlPonter locker){
        locker_ = locker;
    }

    bool notify(){
        std::lock_guard<std::mutex> lg(state_mutex_);
        
        if(state_ != READY){
            return false;
        }
        state_ = START;
        cv_.notify_one();

        return true;
    }

    State getState(){
        return state_;
    }
    

};

}

#endif