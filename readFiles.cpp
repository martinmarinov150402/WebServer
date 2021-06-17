#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

pair<string,string> getMethodAndPath(char buffer[])
{
    string method="",path="";
    bool methodFound=false, pathFound = false;
    for(int i=0;i<strlen(buffer);i++)
    {
        if(buffer[i]==' ')
        {
            if(!methodFound)
            {
                methodFound=true;
            }
            else if(!pathFound)
            {
                pathFound=true;
            }
        }
        else
        {
            if(!methodFound)
            {
                method+=buffer[i];
            }
            else
            {
                if(!pathFound)
                {
                    path+=buffer[i];
                }
                else
                {
                    break;
                }
            }
        }
    }
    return make_pair(method,path);
}
string readFile(string fileName)
{
    ifstream htmlCode("./staticFiles/"+fileName);
    string result = "";
    string buffer;
    if(htmlCode.is_open())
    {
        while(getline(htmlCode,buffer))
        {
            result+=buffer;
        }
    }
    htmlCode.close();
    return result;

}