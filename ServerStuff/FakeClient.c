#include "Server.h" 

int main()
{
    char buff[256];
    char buff2[256];
    Request request;
    strcpy(request.data1, "test");
    strcpy(request.data2, "pass");
    request.requesttype = RegisterReq;
    Init();
    if(Handle(request, buff) == 0)
    {
        printf("success %s\n", buff);
    }
    else
        printf("fail %s\n", buff);
    
    
    
}