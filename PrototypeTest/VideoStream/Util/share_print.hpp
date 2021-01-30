#ifndef SHARE_PRINT_HPP
#define SHARE_PRINT_HPP

//a thread safe print

#include <iostream>
#include <mutex>
#include <string>

std::mutex printMutex;

void share_print(std::string str){
    std::lock_guard<std::mutex> guard(printMutex);
    std::cout<<str<<std::endl;
}

#endif