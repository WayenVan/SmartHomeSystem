#ifndef _MYUTILS_HPP_
#define _MYUTILS_HPP_

//a thread safe print
#include <iostream>
#include <mutex>
#include <string>

namespace myUtils{

static std::mutex printMtx;
static void share_print(std::string str){
    std::lock_guard<std::mutex> guard(printMtx);
    std::cout<<str<<std::endl;
}

static std::mutex cerrMtx;
static void share_cerr(std::string str){
    std::lock_guard<std::mutex> guard(cerrMtx);
    std::cerr<<str<<std::endl;
}


}

#endif