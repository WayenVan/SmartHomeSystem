#ifndef _SERVER_UNIVERAL_HPP_
#define _SERVER_UNIVERAL_HPP_

#include<CppThread.hpp>
#include<my_utils.hpp>
#include<lock_control.hpp>


namespace wayenvan{
/**
 * a server which accept universal order from the application, except video
 */
class ServerUniversal: public CppThread{

    private:
    ServerUniversal& operator=(ServerUniversal& s);
    ServerUniversal(ServerUniversal& s);

    const char* kServerIp_;
    const int kServerPort_;
    const int kMaxEvents_;

    protected:

    //the running server
    void run();

    // @todo register list of other module;
    LockControl::LockControlPonter lock_control_;

    public:
    /**
     * constructor
     */
    ServerUniversal(const char* KServerIp, const int kServerPort):
        kServerIp_(KServerIp),
        kServerPort_(kServerPort),
        kMaxEvents_(10),
        lock_control_(nullptr)
    {
        myUtils::share_print("the universal server initialized");
    }

    void registerModules(LockControl::LockControlPonter lock_control){
        lock_control_ = lock_control;
    }

    


};
}


#endif