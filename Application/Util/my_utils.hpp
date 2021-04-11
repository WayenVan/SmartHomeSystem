#ifndef _MY_UTILS_HPP_
#define _MY_UTILS_HPP_

#include<string>

namespace wayenvan{

namespace myUtils{

void share_print(std::string str);
void share_cerr(std::string str);

template<typename T>
inline const char* get_type(const T& var){
   return typeid(var).name();
}

}

}

#endif