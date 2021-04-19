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

#include<servo.hpp>
#include<pigpio.h>
#include<softPwm.h>
#include<module_exception.hpp>
#include<iostream>
#include<cassert>
#include<unistd.h>
#include<thread>
#include<chrono>

using namespace wayenvan;
using namespace std;

const int port = 17;

int main(int argv, char** argc){

    //test pigpio
    gpioInitialise();
    gpioSetMode(port, PI_OUTPUT);
    gpioSetPWMfrequency(port, 50);
    gpioSetPWMrange(port, 20000);
    gpioPWM(port, 1500);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    gpioPWM(port, 2500);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));

    //test  servo class
    Servo* s2 = new Servo(17);
    bool result1 = s2->reposition(-1);
    assert(result1 == false);
    bool result2 = s2->reposition(181);
    assert(result2 == false);
    delete s2;

    //test multi thread process
    thread t1([&](){
          //test  servo class
        Servo s(17);
        s.reposition(45);
        s.reposition(90);
        s.reposition(0);

    });

    thread t2([&](){
        Servo s(27);
        s.reposition(45);
        s.reposition(180);
        s.reset();
    });

    t1.join();
    t2.join();
    

    gpioTerminate();
    
    return 1;
}

