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
#include<thread>
#include<iostream>
#include<pigpio.h>

using namespace std;
using namespace wayenvan;

int main(int argv, char** argc){

    gpioInitialise();

    LockControl lc(17);

    thread t1([&](){
        while(1){
        this_thread::sleep_for(chrono::seconds(3));
        cout<<"t1: " + to_string(lc.notify())<<endl;
        }
    });


    thread t2([&](){
        while(1){
        this_thread::sleep_for(chrono::seconds(8));
        cout<<"t2: " + to_string(lc.notify())<<endl;
        }
    });

    lc.start();

    lc.join();
    t1.join();
    t2.join();

    gpioTerminate();
}