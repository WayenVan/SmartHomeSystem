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

#include<python3.7/Python.h>
#include<iostream>
#include<bme680.hpp>

using namespace wayenvan;
using namespace std;

int main(){
    Py_Initialize();
    
    BME680* b = BME680::getInstance(); 
    int i = 0;
    while(i<20){
        cout<<b->getTemperature()<<endl;
        cout<<b->getPressure()<<endl;
        cout<<b->getHumidity()<<endl;
        sleep(1);
        i++;
    }
    Py_Finalize();
    return 0;
}