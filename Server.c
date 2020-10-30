#include "AnnonServices.h"
#include "requests.h"

void Init()
{
    DropTables();
    InitTables();
}

int Handle(Request request, char* response)
{
    switch (request.requesttype)
    {
    case RegisterReq:
        if(RegisterService(response, request.data1, request.data2) == 0)
        {
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Register Failure\n");
            return -1;
        }
        break;
    case LoginReq:
    if(LoginService(response, request.data1, request.data2) == 0)
    {
        return 0;
    }
    else
    {
        strcpy(response, "$FAIL$ Login Failure\n");
        return -1;
    }
        break;
    case PostSSNReq:
        if(PostSSN(request.data1, request.authtoken) == 0)
            return 0;
        else
        {
            strcpy(response, "$FAIL$ Post SSN Failure\n");
        }
        break;
    case PostCCNReq:
        if(PostSSN(request.data1, request.authtoken) == 0)
            return 0;
        else
        {
            strcpy(response, "$FAIL$ Post CCN Failure\n");
        }
        
        break;
    case PostPasswordReq:
        break;
    case GetSSNReq:
        break;
    case GetCCNReq:
        break;
    case GetPasswordReq:
        break;
    case DeleteAccountReq:
        break;
    case DeleteBatchReq:
        break;
    case DeleteSSNReq:
        break;
    case DeleteCCNReq:
        break;
    case DeletePasswordReq:
        break;
    default:
        break;
    }
}
