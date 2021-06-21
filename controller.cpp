#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include <iostream>
#include <map>

class Controller
{
    private:
        int socketId;
        Router* routerPtr;
        void (*helloPtr)(map<string,string>);
    public:
        void helloCustomRoute(map<string,string>params)
        {
            string text = "Zdraveite tova e custom route";
            ostringstream repSS;
            repSS<<text.size();
            string reply = okHeaders(repSS.str())+text;
            send(this->socketId, reply.c_str(), strlen(reply.c_str()),0);
        }
        Controller(int sockId, Router* ptr)
        {
            this->socketId = sockId;
            this->routerPtr = ptr;
            insertRoute(this->routerPtr,"POST:/testCustomRoute",helloPtr);   
        }
};
