#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include "readFiles.cpp"
#include "headerGen.cpp"
#include <iostream>

#define PORT 3000
string notFoundReply()
{
    string index = readFile("notFound.html");
    int ContentLength = index.size();
    ostringstream CLSS;
    CLSS<<ContentLength;
    string reply=notFoundHeaders(CLSS.str())+index;
    return reply;
}
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int opt=1;
    int addrlen = sizeof(address);
    //if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(sock, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("Can't bind");
        exit(EXIT_FAILURE);
    }
    string reply="";
    while(1)
    {

        if(listen(sock,300000000)<0)
        {
            perror("Listen");
            exit(EXIT_FAILURE);
        }
        int new_connection;
        if((new_connection = accept(sock, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
        {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }
       
        int valReader;
        char buffer[4096]= {0};
        //valReader = read(sock, buffer, 1024);
        recv(new_connection, (char*)&buffer, sizeof(buffer), 0);
        pair<string,string>metPath = getMethodAndPath(buffer);
        if(new_connection)
        {
            if(metPath.first!="GET")
            {
                reply = notFoundReply();
            }
            else
            {
                reply = "";
                if(metPath.second=="/")
                {
                    metPath.second="index.html";
                }
                else
                {
                    metPath.second = metPath.second.substr(1,metPath.second.size()-1);
                }
                string index = readFile(metPath.second);
                int ContentLength = index.size();
                ostringstream CLSS;
                CLSS<<ContentLength;
                if(index!="")
                {
                    reply=okHeaders(CLSS.str())+index;
                }
                else
                {
                    reply=notFoundReply();   
                }
            }

        }
        printf("%s\n",buffer);

        send(new_connection, reply.c_str(), strlen(reply.c_str()),0);
        close(new_connection);
        //continue;
    }

}