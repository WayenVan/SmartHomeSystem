#ifndef _MYUTILS_HPP_
#define _MYUTILS_HPP_

//a thread safe print
#include <iostream>
#include <mutex>
#include <string>
#include <module_exception.hpp>
#include <typeinfo>

namespace wayenvan{

namespace myUtils{

inline void share_print(std::string str){
    static std::mutex kprint_mutex;
    std::lock_guard<std::mutex> guard(kprint_mutex);
    std::cout<<str<<std::endl;
}

inline void share_cerr(std::string str){
    static std::mutex kshare_cerr_mutex;
    std::lock_guard<std::mutex> guard(kshare_cerr_mutex);
    std::cerr<<str<<std::endl;
}

template<typename T>
inline const char* get_type(const T& var){
   return typeid(var).name();
}

}
}
#endif