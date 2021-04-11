#ifndef _BME680_HPP_
#define _BME680_HPP_

#include<python2.7/Python.h>
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

class Bme680{

    typedef std::lock_guard<std::mutex> Guard;

    private:
    PyObject* module_;
    PyObject* sensor_;
    const std::string kBME680I2CAddr_;

    //mutex
    std::mutex mutex_;
    

    //setting the chip when initailization
    void setting(PyObject* module_dict){
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_humidity_oversample"),
            PyDict_GetItemString(module_dict, "OS_2X"),
            NULL
            );
    
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_pressure_oversample"),
            PyDict_GetItemString(module_dict, "OS_4X"),
            NULL
            );
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_temperature_oversample"),
            PyDict_GetItemString(module_dict, "OS_8X"),
            NULL
            );
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_filter"),
            PyDict_GetItemString(module_dict, "FILTER_SIZE_3"),
            NULL
            );



        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_gas_status"),
            PyDict_GetItemString(module_dict, "ENABLE_GAS_MEAS"),
            NULL
            );
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_gas_heater_temperature"),
            PyLong_FromLong(320),
            NULL
            );
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("set_gas_heater_duration"),
            PyLong_FromLong(150),
            NULL
            );
        PyObject_CallMethodObjArgs(sensor_, 
            PyString_FromString("select_gas_heater_profile"),
            PyLong_FromLong(0),
            NULL
            );

    }

    /**
     * constructor
     */
    Bme680():
        module_(nullptr),
        sensor_(nullptr),
        kBME680I2CAddr_("0x77")
    {
        module_ = PyImport_ImportModule("bme680");

        PyObject *klass = PyObject_GetAttrString(module_, "BME680");
        PyObject *args = PyTuple_New(0);
        PyObject *kargs = PyDict_New();
        PyDict_SetItemString(kargs, "i2c_addr", PyLong_FromSize_t(0x77));
        sensor_ = PyInstance_New(klass, args, kargs);
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

    static Bme680* getInstance(){
        static Bme680 instance;
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
