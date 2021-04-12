#ifndef _AMG8833_HPP_
#define _AMG8833_HPP_

#include<stddef.h>
#include<python3.7m/Python.h>
#include<thread>
#include<module_exception.hpp>

namespace wayenvan{

class AMG8833{

    private:

    PyObject* module_;
    PyObject* sensor_;

    const size_t kAMG8833Addr_;

    /**
     * constructor
     */
    AMG8833(size_t addr = 0x69):
        kAMG8833Addr_(addr)
    {
        //init the class in python
        module_ = PyImport_ImportModule("adafruit_amg88xx");
        PyObject *board = PyImport_ImportModule("board");

        PyObject * I2C_func = PyObject_GetAttrString(board, "I2C");
        PyObject * i2c = _PyObject_CallNoArg(I2C_func);

        PyObject *klass = PyObject_GetAttrString(module_, "AMG88XX");

        PyObject *args = PyTuple_New(1);
        int res = PyTuple_SetItem(args, 0, i2c);

        PyObject *kargs = PyDict_New();
        PyDict_SetItemString(kargs, "addr", PyLong_FromSize_t(kAMG8833Addr_));

        sensor_ = PyObject_Call(klass, args, kargs);

        if(sensor_ == nullptr){
            ModuleException e(myUtils::get_type(this), "can not initialize the bme680 sensor");
            throw e;
        }
    }

    public:
    /**
     * using singleton parttern
     */

    static AMG8833* getInstance(){
        static AMG8833 instance;
        return &instance;
    }

    /**
     * get the temperature of one pixel
     * @brief the amg88xx generate a matrix of temperature, thus to find on pixel
     * for our intended human temperature
     */

    double getTemperature(int row_positioin, int col_position){
        PyObject *pixels = PyObject_GetAttrString(sensor_, "pixels");
        PyObject *row = PyList_GetItem(pixels, row_positioin);
        PyObject *tem = PyList_GetItem(row, col_position);
        return PyFloat_AsDouble(tem);
    }


};


}

#endif 