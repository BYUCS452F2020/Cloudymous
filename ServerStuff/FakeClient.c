#include "Server.h" 

int main()
{
    Request request;
    Response response;
    strcpy(request.data1, "test");
    strcpy(request.data2, "pass");
    request.requesttype = RegisterReq;
    Init();
    if(Handle(request, &response) == 0)
    {
        printf("success %s\n", response.data1);
    }
    else
        printf("fail %s\n", response.data1);

}