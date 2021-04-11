//a thread safe print
#include <my_utils.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <typeinfo>

using namespace std;

namespace wayenvan{

namespace myUtils{

mutex kprint_mutex;
void share_print(string str){
    lock_guard<mutex> guard(kprint_mutex);
    cout<<str<<endl;
}


mutex kshare_cerr_mutex;
void share_cerr(string str){
    lock_guard<mutex> guard(kshare_cerr_mutex);
    cerr<<str<<endl;
}

}


}
