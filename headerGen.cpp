#include <iostream>
#include <string>
#include <ctime>

using namespace std;

string okHeaders(string contentLength)
{
    time_t now = time(0);
    string headers = 
    string("HTTP/2.0 200 OK\n")+
    string("Server: MartoWeb 1.0.0\n")+
    string("Date: ")+string(ctime(&now))+
    string("ETag: \"56d-9989200-1132c580\"\n")+
    string("Content-Type: text/html\n")+
    string("Content-Length: ")+contentLength+string("\n")+
    string("Accept-Ranges: bytes\n")+
    string("Connection: open\n")+
    string("\n");
    return headers;
}
string notFoundHeaders(string contentLength)
{
    time_t now = time(0);
    string headers = 
    string("HTTP/2.0 404 Not Found\n")+
    string("Server: MartoWeb 1.0.0\n")+
    string("Date: ")+string(ctime(&now))+
    string("ETag: \"56d-9989200-1132c580\"\n")+
    string("Content-Type: text/html\n")+
    string("Content-Length: ")+contentLength+string("\n")+
    string("Accept-Ranges: bytes\n")+
    string("Connection: open\n")+
    string("\n");
    return headers;
}