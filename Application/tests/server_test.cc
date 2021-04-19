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

#include <camera.hpp>
#include <my_utils.hpp>
#include <server_video.hpp>
#include <memory>
#include <module_exception.hpp>
#include <python3.7m/Python.h>
#include <pigpio.h>

using namespace std;
using namespace wayenvan;

int main (){

    typedef unique_ptr<ServerVideo> ServerVideo_ptr;

    Py_Initialize();
    gpioInitialise();

    try{
        
        myUtils::share_print("hello");
        Detector::DetectorPointer detector =Detector::DetectorPointer(new Detector("/home/pi/Documents/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml"));
        ServerVideo_ptr server = ServerVideo_ptr(new ServerVideo("12345"));
        DetectManager::DetectManagerPointer detect_manager = DetectManager::DetectManagerPointer(new DetectManager());
        LockControl::LockControlPonter lock_control = LockControl::LockControlPonter(new LockControl(17));

        server->registerDetector(detector);
        detector->regsisterDetectManager(detect_manager);
        detect_manager->registerLockControl(lock_control);

        detector->start();
        server->start();
        lock_control->start();

        lock_control->join();
        detect_manager->join();
        detector->join();
        server->join();


    }catch(const ModuleException& e){
        myUtils::share_cerr("Error ocurr in "+string(e.get_error_module_name())+":");
        myUtils::share_cerr(e.what());
    }
    gpioTerminate();
    Py_Finalize();

    myUtils::share_print("finished!");
}