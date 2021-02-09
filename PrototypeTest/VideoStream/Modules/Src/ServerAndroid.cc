#include <ServerAndroid.hpp>
#include <myUtils.hpp>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>

#include <opencv2/core.hpp>

#include <thread>



void ServerAndroid::run(){

    //create socket
    int listening = socket(AF_INET , SOCK_STREAM, 0);
    if (listening== -1){
        myUtils::share_cerr("Can not create a soket!");
        exit(0);
    }

    //Bind the socket to a IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(12345);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if(bind(listening, (sockaddr*)&hint, sizeof(hint))==-1)
    {
        myUtils::share_cerr("can not bind to IP/port");
        exit(0);
    }

    //mark the socket for listening in
    if(listen(listening, SOMAXCONN)==-1){
        myUtils::share_cerr("can not listening");
        exit(0);
    }

    //Accept a call 
    sockaddr_in client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if(clientSocket == -1){
        myUtils::share_cerr("problem with client connecting!");
        exit(0);
    }

    //close the listening socket
    close(listening);

    //print connection 
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo(
        (sockaddr*)&client,
        sizeof(client),
        host,
        NI_MAXHOST,
        svc,
        NI_MAXSERV,
        0
    );

    if(result){
        std::string info ="host:"+std::string(host)+"server:"+std::string(svc);
        myUtils::share_print(info);
    }

    char buf[4096];
    //while receiving display message, echo message
    while(true){
        memset(buf,0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if(bytesRecv == -1){
            myUtils::share_cerr("there was a connection issue");
            break;
        }
        if(bytesRecv == 0){
            myUtils::share_print("the client disconnected");
            break;
        }

        cv::Mat frame;
        if(this->detector->bufferPop(frame)){
            char temp[1024];
            sprintf(temp, "wo hao cong ming %ld \n", frame.dims);
            send(clientSocket, temp, sizeof(temp), 0); 
        }
        else{
            char temp[1024] = "did not get frame!";
            send(clientSocket, temp, sizeof(temp)+1, 0); 
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(30));
    }
    //close socket
}