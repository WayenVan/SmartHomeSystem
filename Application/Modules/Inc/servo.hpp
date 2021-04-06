#ifndef _SERVO_HPP
#define _SERVO_HPP

#include<stdint.h>
#include<module_exception.hpp>
#include<my_utils.hpp>
#include<string>
#include<pigpio.h>
#include<thread>
#include<chrono>

/**
 * @brief this is a servo control class, which is not thread safe,
 * the gpio port must initialized by gpioInitialize() function,
 * with which we inside the room.
 */
namespace wayenvan{

class Servo {

    private:
    Servo& operator=(const Servo& c);
    Servo(const Servo& c);

    /**
     * data
     */

    const int kGpioPort_;
    int duty_per_degree_;
    

    public:
    /**
     * constructor
     * @brief every thing must be initialized by 
     */
    Servo(int gpio_port): 
        kGpioPort_(gpio_port)
    {
        if(kGpioPort_ > 31 || kGpioPort_ < 0){
            ModuleException e(myUtils::get_type(this), "gpio port out of range!");
            throw e;
        }

        //calculate the servo duty of 1 degree.
        duty_per_degree_ = 1000/90;

        //initialization the gpio port
        gpioSetMode(kGpioPort_, PI_OUTPUT);
        gpioSetPWMfrequency(kGpioPort_, 50);
        gpioSetPWMrange(kGpioPort_, 20000);
        reset();

        std::string s = "the servo in port " + std::to_string(gpio_port) + " initialize successfully!";
        myUtils::share_print(s);

    }
    ~Servo(){
    }

    /**
     * @brief block reposition function of sg90,
     * it will block the thread for 500ms to manipulate the sg90 servo,
     * @return return true if success
     */
    bool reposition(int angle){

        if(angle<=0 || angle> 180){
            myUtils::share_print("reposition angle of servo out of range in pin " + std::to_string(kGpioPort_));
            return false;
        }
        gpioPWM(kGpioPort_, 500+duty_per_degree_*angle);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return true;
    }

    void reset() {
        gpioPWM(kGpioPort_, 500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

};

}

#endif