#ifndef _BME680_HPP_
#define _BME680_HPP_

#include<python3.7m/Python.h>
#include<my_utils.hpp>
#include<string>
#include<iostream>
#include<mutex>
#include<module_exception.hpp>


namespace wayenvan{

/**
 * a thread safe bme680 driver class,
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
     */
    BME680(size_t addr = 0x77):
        module_(nullptr),
        sensor_(nullptr),
        kBME680I2CAddr_(0x77)
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
