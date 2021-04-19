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

#ifndef _BME680_HPP_
#define _BME680_HPP_

#include<python3.7m/Python.h>
#include<my_utils.hpp>
#include<string>
#include<iostream>
#include<mutex>
#include<module_exception.hpp>
#include"../main_defs.hpp"

#define BME680_ADDR 0x77

namespace wayenvan{

/**
 * @brief a thread safe bme680 driver class,
 * which implemented by python
 */

class BME680{

    typedef std::lock_guard<std::mutex> Guard;

    private:
    PyObject* module_;
    PyObject* sensor_;
    const size_t kBME680I2CAddr_;

    //mutex
    std::mutex mutex_;

    /**
     * constructor
     * @brief this initialize all modules by python
     */
    BME680(size_t addr = BME680_ADDR):
        module_(nullptr),
        sensor_(nullptr),
        kBME680I2CAddr_(addr)
    {
        module_ = PyImport_ImportModule("bme680");

        PyObject *klass = PyObject_GetAttrString(module_, "BME680");
        PyObject *args = PyTuple_New(0);
        PyObject *kargs = PyDict_New();
        PyDict_SetItemString(kargs, "i2c_addr", PyLong_FromSize_t(kBME680I2CAddr_));
        sensor_ = PyObject_Call(klass, args, kargs);
        //PyObject *module_dict = PyModule_GetDict(module_);
        //setting(module_dict);
        if(sensor_ == nullptr){
            ModuleException e(myUtils::get_type(this), "can not initialize the bme680 sensor");
            throw e;
        }
    }

    public:

    /**
     * using singleton parttern
     */

    static BME680* getInstance(){
        static BME680 instance;
        return &instance;
    }

    double getTemperature(){
        Guard guard(mutex_);
        PyObject_Call(PyObject_GetAttrString(sensor_, "get_sensor_data"), PyTuple_New(0), NULL);
        PyObject * res = PyObject_GetAttrString(PyObject_GetAttrString(sensor_, "data"), "temperature");
        return PyFloat_AsDouble(res);
    }

    
    double getPressure(){
        Guard guard(mutex_);
        PyObject_Call(PyObject_GetAttrString(sensor_, "get_sensor_data"), PyTuple_New(0), NULL);
        PyObject * res = PyObject_GetAttrString(PyObject_GetAttrString(sensor_, "data"), "pressure");
        return PyFloat_AsDouble(res); 
    }

    double getHumidity(){
        Guard guard(mutex_);
        PyObject_Call(PyObject_GetAttrString(sensor_, "get_sensor_data"), PyTuple_New(0), NULL);
        PyObject * res = PyObject_GetAttrString(PyObject_GetAttrString(sensor_, "data"), "humidity");
        return PyFloat_AsDouble(res); 
    }


};

}

#endif
