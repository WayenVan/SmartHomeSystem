#include <server_video.hpp>
#include <my_utils.hpp>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <thread>

namespace wayenvan{

void ServerVideo::run(){

    //create socket
    int listening = socket(AF_INET , SOCK_STREAM, 0);
    if (listening== -1){
        myUtils::share_cerr("Can not create a soket!");
        exit(0);
    }

    //Bind the socket to a IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->kServerPort_);
    inet_pton(AF_INET, this->kServerIp_, &hint.sin_addr);


    if(::bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
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

    //accept loop
    while(true){

        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if(clientSocket == -1){
            myUtils::share_cerr("problem when accept connection!");
            exit(0);
        }

        //close the listening socket
        //close(listening);

        //print information 
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
            myUtils::share_print(host);
        }


        //recieve buffer
        char bufRecv[1024];
        std::vector<uchar> buffer;
        int size = 0;
        cv::Mat frame;
        //while receiving display message, echo message
        while(true){

            //clear buffer and restart transmission
            memset(bufRecv,0, 1024);
            buffer.clear();
            size = 0;

            //recieve data from client and check if it is connected
            int bytesRecv = recv(clientSocket, bufRecv, 4096, 0);
            if(bytesRecv == -1){
                myUtils::share_cerr("the client disconnected not safe");
                exit(0);
            }
            if(bytesRecv == 0){
                myUtils::share_print("the client disconnected");
                break;
            }

            //send frame to client
            if(this->detector->bufferPop(frame)){
                
                cv::imencode(".jpg", frame, buffer);
                //sprintf(bufSendPtr.get(), "frame length: %d frame width: %d, encode size: %lu\n", frame.cols, frame.rows, buffer.size());

                //firstly send image size
                size = buffer.size();
                if(send(clientSocket, &size, sizeof(int), 0)== -1){
                    myUtils::share_print("problem when sending information");
                    break;
                }

                //secondly send the image
                if(send(clientSocket, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0)== -1){
                    myUtils::share_print("problem when sending information");
                    break;
                }
                myUtils::share_print("send an image, buffer size: "+std::to_string(buffer.size()));
            }else{
                if(send(clientSocket, &size, sizeof(int), 0)== -1){
                    myUtils::share_print("problem when sending information");
                    break;
                }
            }
            
            std::this_thread::sleep_for (std::chrono::milliseconds(30));
        }
        //close socket
        close(clientSocket);
    }
}

}