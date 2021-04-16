#include <server_video.hpp>
#include <my_utils.hpp>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <sys/fcntl.h>
#include <sys/epoll.h>


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <thread>
#include <module_exception.hpp>

namespace wayenvan{

#define MAXEVENTS 4

void ServerVideo::run(){

    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;

    sfd = createAndBind(kServerPort_);
    if (sfd == -1)
    abort ();

    s = makeSocketNonBlocking(sfd);
    if (s == -1)
    abort ();

    s = listen (sfd, SOMAXCONN);
    if (s == -1)
    {
        perror ("listen");
        abort ();
    }

    efd = epoll_create1 (0);
    if (efd == -1)
    {
        perror ("epoll_create");
        abort ();
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
    {
        perror ("epoll_ctl");
        abort ();
    }

    /* Buffer where events are returned */
    events = (epoll_event*)calloc (MAXEVENTS, sizeof(epoll_event));

    /* The event loop */
    while (1)
    {
        int n, i;

        n = epoll_wait (efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++)
    {
        if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN)))
        {
                /* An error has occured on this fd, or the socket is not
                    ready for reading (why were we notified then?) */
            fprintf (stderr, "epoll error\n");
            close (events[i].data.fd);
            continue;
        }

        else if (sfd == events[i].data.fd)
        {
                /* We have a notification on the listening socket, which
                    means one or more incoming connections. */
                while (1)
                {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    in_len = sizeof in_addr;
                    infd = accept (sfd, &in_addr, &in_len);
                    if (infd == -1)
                    {
                        if ((errno == EAGAIN) ||
                            (errno == EWOULDBLOCK))
                        {
                            /* We have processed all incoming
                                connections. */
                            break;
                        }
                        else
                        {
                            perror ("accept");
                            break;
                        }
                    }

                    s = getnameinfo (&in_addr, in_len,
                                    hbuf, sizeof hbuf,
                                    sbuf, sizeof sbuf,
                                    NI_NUMERICHOST | NI_NUMERICSERV);
                    if (s == 0)
                    {
                        printf("Accepted connection on descriptor %d "
                                "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                    /* Make the incoming socket non-blocking and add it to the
                        list of fds to monitor. */
                    s = makeSocketNonBlocking (infd);
                    if (s == -1)
                    abort ();

                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1)
                    {
                        perror ("epoll_ctl");
                        abort ();
                    }
                }
                continue;
            }
            else
            {
                /* We have data on the fd waiting to be read. Read and
                    display it. We must read whatever data is available
                    completely, as we are running in edge-triggered mode
                    and won't get a notification again for the same
                    data. */
                int done = 0;
                int recv_size = 0;
                char recv_data[512];
                while (1)
                {
                    ssize_t count;

                    //recv buff
                    char buf[512];
                    count = read (events[i].data.fd, buf, sizeof buf);

                    if (count == -1)
                    {
                        /* If errno == EAGAIN, that means we have read all
                            data. So go back to the main loop. */
                        if (errno != EAGAIN)
                        {
                            perror ("read");
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        /* End of file. The remote has closed the
                            connection. */
                        done = 1;
                        break;
                    }
                    else if (count >=0){
                        std::memcpy(recv_data + recv_size, buf, count);
                        recv_size += count;
                    }
                    
                }

                int instruction = 0x00;

                if(recv_size > 4){
                    myUtils::share_print("recieve int but exceed");
                }

                std::memcpy(&instruction, recv_data, recv_size);
                myUtils::share_print("video server get " + std::to_string(recv_size) + " byte");
                myUtils::share_print("video server get " + std::to_string(instruction) + " instruction");

              
                /* todo sending frame */
                cv::Mat frame;
        
                if(detector->bufferPop(frame)){
                    done = sendFrame(events[i].data.fd, frame);
                }else{
                    //if taking hte frame unsuccessful
                    while(1){
                        int s = 0;
                        int result = send(events[i].data.fd, &s, sizeof(int), 0);

                        if(result <= 0){

                            if(errno != EAGAIN || EWOULDBLOCK){
                                perror ("send 0 frame size");
                                done = 1;
                            }
                            continue;
                        }
                        if(result > 0){
                            break;
                        }
                    }
                }
                    
                if(done == 1) break;
                
            
            

                if (done)
                {
                    printf ("Closed connection on descriptor %d\n",
                            events[i].data.fd);

                    /* Closing the descriptor will make epoll remove it
                        from the set of descriptors which are monitored. */
                    close (events[i].data.fd);
                }

            }
        }
    }

    free (events);

    close (sfd);
    }

int ServerVideo::sendFrame(const int socket, cv::Mat& frame){

    std::vector<uchar> buffer;
    cv::imencode(".jpg", frame, buffer);
    int buffer_size = buffer.size();
    
    //send the frame size
    while(1){

        int result = send(socket, &buffer_size, sizeof(int), 0);
        if(result <= 0){
            if(errno != EAGAIN || EWOULDBLOCK){
                perror ("send frame size");
                return 1;
            }
            continue;
        }

        if(result > 0){
            myUtils::share_print("send size " + std::to_string(result));
            break;
        }
    }

    //send the frame
    while(1){
        int result = send(socket, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
        
        if(result <= 0){
            if(errno != EAGAIN || EWOULDBLOCK){
                perror ("send frame");
                return 1;
            }
            continue;
        }

        if(result > 0){
            break;
        }
    }

    return false;
}

int ServerVideo::createAndBind(const char* port){

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
    {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
    return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
    sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
    continue;

    s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
    if (s == 0)
    {
        /* We managed to bind successfully! */
        break;
    }

    close (sfd);
    }

    if (rp == NULL)
    {
        fprintf (stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo (result);

    return sfd;
}

int ServerVideo::makeSocketNonBlocking(const int& sfd){

    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

}