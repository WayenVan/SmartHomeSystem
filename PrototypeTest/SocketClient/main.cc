#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define SERVER_PORT 12345


int main(int argc, char** argv){
    
    int skt = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "192.168.0.104", &server_address.sin_addr);
    
    //connect
    int server_socket = connect(skt, (sockaddr*)&server_address, sizeof(sockaddr_in));

    if(server_socket == -1){
        std::cerr<<"can not connect"<<std::endl;
        return 0;
    }
    
    const int buf_send = 0x11;
    int img_size;
    unsigned char buf_img[128*1024];
    std::vector<uchar> buf_img_vec;

    //window initialize
    cv::namedWindow("Frame");

    while(true){

        //clear bufffer
        memset(&img_size, 0, sizeof(int));
        memset(buf_img, 0, 128*1024);
        

        send(skt, &buf_send, sizeof(buf_send), 0);

        usleep(1000*60);
        recv(skt, &img_size, sizeof(int), MSG_WAITALL);
        std::cout<<"frame size:"<<img_size<<std::endl;
        if(img_size > 0){
           
            int result = recv(skt, &buf_img, img_size, MSG_WAITALL);

            std::cout<<"frame actual size:"<<result<<std::endl;
            // Display the resulting frame
            buf_img_vec.assign(buf_img, buf_img+img_size);
            cv::Mat frame = cv::imdecode(cv::Mat(buf_img_vec), -1);
            cv::imshow( "Frame", frame);
            cv::waitKey(60);
        }

    }

    
    
    return 0;
}