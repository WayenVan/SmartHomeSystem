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

#include<iostream>
#include<amg8833.hpp>
#include<python3.7m/Python.h>
#include<pigpio.h>

using namespace wayenvan;
using namespace std;

int main(int argc, char** argv){
    Py_Initialize();
    gpioInitialise();

    AMG8833* a = AMG8833::getInstance();
    sleep(4);
    while(1){
       cout<<a->getTemperature(4, 4)<<endl;
       sleep(1);
    }
  

    Py_Finalize();
}