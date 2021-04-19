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

#ifndef _SERVER_VIDEO_H_
#define _SERVER_VIDEO_H_

#include <CppThread.hpp>
#include <detector.hpp>

namespace wayenvan{

/**
 * a video server provide frame to all clients,
 * using epoll as NIO
 */
class ServerVideo: public CppThread{

    public:
    typedef std::shared_ptr<ServerVideo> ServerVideoPointer;

    private:
    Detector::DetectorPointer detector;

    //change ip
    const char* kServerPort_;

    void run();
    int createAndBind(const char* port);
    int makeSocketNonBlocking(const int& sfd);

    /**
     * send a frame through a socket
     * @brief the socket should be nonblock
     * @return it return 1 if the socket connection occur some unexpected error
     * else return 0
     */
    int sendFrame(const int socket, cv::Mat& frame);

    
    public:
    ServerVideo(const char* server_port): 
        detector(nullptr), 
        kServerPort_(server_port)
    {
        myUtils::share_print("video server initialized");
        
    }
    
    ~ServerVideo() {}

    void registerDetector(Detector::DetectorPointer detector){
        this->detector = detector;
    }

    
};

}

#endif