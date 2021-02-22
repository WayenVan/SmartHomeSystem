#ifndef _MYUTILS_HPP_
#define _MYUTILS_HPP_

//a thread safe print
#include <iostream>
#include <mutex>
#include <string>
#include <module_exception.hpp>

namespace wayenvan{

namespace myUtils{

using namespace std;

inline void share_print(string str){
    static mutex kprint_mutex;
    lock_guard<mutex> guard(kprint_mutex);
    cout<<str<<endl;
}

inline void share_cerr(string str){
    static mutex kshare_cerr_mutex;
    lock_guard<mutex> guard(kshare_cerr_mutex);
    cerr<<str<<endl;
}

}
}
#endif