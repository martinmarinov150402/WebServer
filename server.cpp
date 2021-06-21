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
#include "router.cpp"
#include <map>

#define PORT 3000

int new_connection;
string notFoundReply()
{
    string index = readFile("notFound.html");
    int ContentLength = index.size();
    ostringstream CLSS;
    CLSS<<ContentLength;
    string reply=notFoundHeaders(CLSS.str())+index;
    return reply;
}
void helloCustomRoute(map<string,string>params)
{
    string text = "Zdraveite tova e custom route";
    ostringstream repSS;
    repSS<<text.size();
    string reply = okHeaders(repSS.str())+text;
    send(new_connection, reply.c_str(), strlen(reply.c_str()),0);
}
void initializeRouter(Router* router)
{
    void (*helloPtr)(map<string,string>) = &helloCustomRoute;
    insertRoute(router,"POST:/testCustomRoute",helloPtr);   
}
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int opt=1;
    int addrlen = sizeof(address);
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    Router* router = new Router();
    initializeRouter(router);
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
        
        if((new_connection = accept(sock, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
        {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }
        int valReader;
        char buffer[4096]= {0};
        valReader = read(sock, buffer, 1024);
        recv(new_connection, (char*)&buffer, sizeof(buffer), 0);
        pair<string,string>metPath = getMethodAndPath(buffer);
        if(new_connection)
        {
            //Controller* cont = new Controller(new_connection,router);
            if(metPath.first!="GET")
            {
                map<string,string>a;
                int res = useRoute(router,metPath.first+":"+metPath.second,a);
                if(res==404)
                {
                    string rep = notFoundReply();
                    send(new_connection,rep.c_str(),strlen(rep.c_str()),0);
                }
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