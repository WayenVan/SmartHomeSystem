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

#include<detect_manager.hpp>
#include<lock_control.hpp>
#include<python3.7m/Python.h>
#include<pigpio.h>
#include<thread>
#include<iostream>

using namespace wayenvan;

int main(){

    gpioInitialise();
    Py_Initialize();

    LockControl::LockControlPonter lock_control(new LockControl(17));
    DetectManager::DetectManagerPointer detect_manager(new DetectManager());

    std::thread T([&](){
        while(1){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout<<detect_manager -> notify()<<std::endl;
        }
    });
    

    detect_manager->registerLockControl(lock_control);

    lock_control -> start();
    detect_manager -> start();

    lock_control -> join();
    detect_manager -> join();
    T.join();

    Py_Finalize();
    gpioTerminate();
    return 0;
}