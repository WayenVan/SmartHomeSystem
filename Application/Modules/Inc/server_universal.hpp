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

#ifndef _SERVER_UNIVERAL_HPP_
#define _SERVER_UNIVERAL_HPP_

#include<CppThread.hpp>
#include<my_utils.hpp>
#include<lock_control.hpp>
#include<bme680.hpp>


namespace wayenvan{
/**
 * a server which accept universal order from the application, except video
 * using epoll as a NIO with
 */
class ServerUniversal: public CppThread{
    public:
    typedef std::shared_ptr<ServerUniversal> ServerUniversalPointer;

    private:
    ServerUniversal& operator=(ServerUniversal& s);
    ServerUniversal(ServerUniversal& s);

    const char* kServerPort_;
    const int kMaxEvents_;

    /**
     * handle the instruction recieved 
     * @return return 0 if the connection is still work, 1 if hte connection closed
     */
    int handleInstruction(const int& socket, const int& instruction);

    /** 
     * send double data
     * @brief it convert the data = data * 1000, 
     * than convert it and send as an integer which could be reverse easily
     * @return 0 if socket still validate, 1 if socket is closed
     */
    int sendData(const int& socket, const double& data);


    protected:

    //the running server using epoll 
    void run();

    /**
     * create the listen socket and bind it to the port
     */
    int createAndBind(const char* port);

    /**
     * make the socket not block
     */
    int makeSocketNonBlocking(int sfd);

    // @todo register list of other module;
    LockControl::LockControlPonter lock_control_;
    BME680* gas_sensor_; 

    public:
    /**
     * constructor
     * @par kServerPort: the port of localhost, the server will automatically start with "0.0.0.0" for all ip avialiable in this 
     */
    ServerUniversal(const char* kServerPort):
        kServerPort_(kServerPort),
        kMaxEvents_(10),
        lock_control_(nullptr)
    {
        gas_sensor_ = BME680::getInstance();
        if(gas_sensor_ == nullptr){
            ModuleException e(myUtils::get_type(this), "get gas sensor failed");
            throw e;
        }

        myUtils::share_print("the universal server initialized");
    }

    void registerModules(LockControl::LockControlPonter lock_control){
        lock_control_ = lock_control;
        if(lock_control_.get() == nullptr){
             ModuleException e(myUtils::get_type(this), "lock control regsister failed");
            throw e;
        }
    }

    


};
}


#endif