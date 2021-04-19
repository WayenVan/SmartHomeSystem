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

#ifndef _LOCK_CONTROL_HPP_
#define _LOCK_CONTROL_HPP_

#include<CppThread.hpp>
#include<mutex>
#include<condition_variable>
#include<stdint.h>
#include<servo.hpp>

namespace wayenvan{



/**
 * a nonblock door lock control module
 * @brief this is a new thread which include 3 state:
 * - STATE_READY
 * - STATE_OPENING
 * It only accept opening the door command only if it s at the STATE_READY
 * */
class LockControl: public CppThread{

    typedef std::lock_guard<std::mutex> LockGuard;
    typedef std::unique_lock<std::mutex> UniqueLock;
    
    public:
    typedef enum LockState{
            LOCK_STATE_READY,
            LOCK_STATE_START,
            LOCK_STATE_OPENING
    } State;
    typedef std::shared_ptr<LockControl> LockControlPonter;

    private:
    LockControl(const LockControl& lc);
    LockControl& operator=(const LockControl& lc);

    std::mutex state_mutex_;
    State state_;

    Servo servo_;
    const u_int8_t kServoPinNumber_;
    std::condition_variable cv_;
    

    protected:
    void run();

    public:
    /** 
     * constructor
     * */
    LockControl(u_int8_t servo_pin_number):
        kServoPinNumber_(servo_pin_number),
        state_(LOCK_STATE_READY),
        servo_(servo_pin_number)
    {
        myUtils::share_print("lock_contorl initialized");
    }
    ~LockControl(){}

    /**
     * notify the module to open the lock
     * @return true if the lock is successfully opened, false if the lock is in other state.
     *  */
    bool notify();

    State get_state();
};

}


#endif