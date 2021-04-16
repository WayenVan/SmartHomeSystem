#include<lock_control.hpp>

namespace wayenvan{

bool LockControl::notify(){
    UniqueLock state_mutex(state_mutex_);
    if(state_ != LOCK_STATE_READY){
        return false;
    }
    state_ = LOCK_STATE_START;
    state_mutex.unlock();
    
    cv_.notify_one();
    return true;    
}

LockControl::State LockControl::get_state(){
    LockGuard lock_guard(state_mutex_);
    return  state_;
}

void LockControl::run(){
    myUtils::share_print("lock control start");
    UniqueLock state_mutex(state_mutex_);
    while(1){
        state_ = LOCK_STATE_READY;
        cv_.wait(state_mutex, [this]{ return this->state_ == LOCK_STATE_START;});

        //change status;
        state_ = LOCK_STATE_OPENING;
        state_mutex.unlock();

        //start unlock
        myUtils::share_print("open the door!");
        servo_.reposition(90);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        myUtils::share_print("close the door!");
        servo_.reset();
        state_mutex.lock();
    }
}

}