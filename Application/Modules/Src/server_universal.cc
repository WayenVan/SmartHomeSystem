/* 
 * This file is part of the SmartHomeSystem distribution (https://github.com/WayenVan/SmartHomeSystem).
 * Copyright (c) 2021 Jingyan Wang.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include<server_universal.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>


#define MAXEVENTS 64

using namespace std;

namespace wayenvan{

int ServerUniversal::createAndBind(const char* port){

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

int ServerUniversal::makeSocketNonBlocking(int sfd){

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

void ServerUniversal::run(){

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

            //recieve instruction
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
                      memcpy(recv_data + recv_size, buf, count);
                      recv_size += count;
                  }
                  
              }
            
            if (done)
              {
                printf ("Closed connection on descriptor %d\n",
                        events[i].data.fd);

                /* Closing the descriptor will make epoll remove it
                    from the set of descriptors which are monitored. */
                close (events[i].data.fd);
                break;
              }

              //print instrution
            int instruction = 0x00;

            if(recv_size > 4){
                myUtils::share_print("recieve int but exceed");
            }

            memcpy(&instruction, recv_data, recv_size);
            myUtils::share_print("universal server get " + std::to_string(recv_size) + " byte");
            myUtils::share_print("universal server get " + std::to_string(instruction) + " instruction");

            //handle instruction
            done = handleInstruction(events[i].data.fd, instruction);

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

  ::free (events);

  close (sfd);
}


int ServerUniversal::handleInstruction(const int& socket, const int& instruction){
  
  if(instruction == 0x11){
    double temp = gas_sensor_->getTemperature();
    return sendData(socket, temp);
  }else if(instruction == 0x12){
    double humi = gas_sensor_->getHumidity();
    return sendData(socket, humi);
  }else if(instruction == 0x13){
    double press = gas_sensor_->getPressure();
    return sendData(socket, press);
  }else if(instruction == 0x15){
    lock_control_ -> notify();
  }else if(instruction == 0x16){
    LockControl::State state = lock_control_->get_state();
    return sendData(socket, state == LockControl::LOCK_STATE_OPENING);
  }
  
  return 0;
}

int ServerUniversal::sendData(const int& socket, const double& data){

  int data_temp = data * 1000.0;
  myUtils::share_print(std::to_string(data_temp));
  while(1){

    int count = 0;
    count = send(socket, &data_temp, sizeof(data_temp), 0);

    if(count <= 0){
      if(errno != EAGAIN || EWOULDBLOCK){
          perror ("send frame");
          return 1;
      }
      continue;
    }

    if(count > 0){
        myUtils::share_print("actual send size " + std::to_string(count));
        break;
    }

  }
  return 0;
}



}