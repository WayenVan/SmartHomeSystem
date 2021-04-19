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