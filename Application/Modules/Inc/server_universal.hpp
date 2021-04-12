#ifndef _SERVER_UNIVERAL_HPP_
#define _SERVER_UNIVERAL_HPP_

#include<CppThread.hpp>
#include<my_utils.hpp>
#include<lock_control.hpp>


namespace wayenvan{
/**
 * a server which accept universal order from the application, except video
 * using epoll as a NIO with
 */
class ServerUniversal: public CppThread{

    private:
    ServerUniversal& operator=(ServerUniversal& s);
    ServerUniversal(ServerUniversal& s);

    const char* kServerPort_;
    const int kMaxEvents_;

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
        myUtils::share_print("the universal server initialized");
    }

    void registerModules(LockControl::LockControlPonter lock_control){
        lock_control_ = lock_control;
    }

    


};
}


#endif