#ifndef _SERVERANDROID_HPP_
#define _SERVERANDROID_HPP_

#include <CppThread.h>
#include <Detector.hpp>

class ServerAndroid: public CppThread{

    private:
    Detector* detector;
    void run();
    
    public:
    ServerAndroid(){
    }
    ~ServerAndroid() {}

    void registerDetector(Detector* detector){
        this->detector = detector;
    }
   
    
};

#endif