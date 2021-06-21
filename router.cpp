#include <string>
#include <iostream>
#include <map>

struct Router
{
    bool endOfRoute;
    void (*action)(map<string,string>);
    Router* arr[255];
    Router()
    {
        //socketId=0;
        action=nullptr;
        endOfRoute=false;
    }
};
void insertRoute(Router* a, string route, void (*fun_ptr)(map<string,string>))
{

    for(int i=0;i<route.size();i++)
    {
        if(!a->arr[int(route[i])])
        {
            a->arr[int(route[i])] = new Router();  
            a=a->arr[int(route[i])]; 
            //if(!a->endOfRoute)a->socketId=socket;
        }
        else
        {
            a=a->arr[int(route[i])];
        }
        if(i==route.size()-1)
        {
            a->endOfRoute=true;
            a->action=fun_ptr;
        }
    }
}
int useRoute(Router *a, string route,map<string,string>params)
{
    for(int i=0;i<route.size();i++)
    {
        if(!a->arr[int(route[i])])
        {
            return 404;
        }
        else
        {
            a = a->arr[int(route[i])];
        }
        if(i==route.size()-1)
        {
            if(a->endOfRoute)
            {
                a->action(params);
                return 200;
            }
            else
            {
                return 404;
            }
        }
    }
    return 200;
}
