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

#include<server_universal.hpp>
#include<python3.7m/Python.h>
#include<pigpio.h>

using namespace std;
using namespace wayenvan;

int main(int argc, char** argv){

    Py_Initialize();
    gpioInitialise();
    
    LockControl::LockControlPonter lc(new LockControl(17));
    ServerUniversal::ServerUniversalPointer s(new ServerUniversal("1148"));
    
    s -> registerModules(lc);
    
    s->start();
    lc->start();

    lc->join();
    s->join();

    gpioTerminate();
    Py_Finalize();

}