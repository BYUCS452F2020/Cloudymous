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
            return -1;
        }
        
        break;
    case PostPasswordReq:
        if(PostPassword(request.data1, request.authtoken) == 0)
            return 0;
        else
        {
            strcpy(response, "$FAIL$ Post Password Failure\n");
            return -1;
        }
        break;
    case GetSSNReq:
        if(GetSSN(response, request.data1))
        {
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Get SSN Failure\n");
            return -1;
        }
        
        break;
    case GetCCNReq:
        if(GetCCN(response, request.data1))
        {
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Get CCN Failure\n");
            return -1;
        }
        break;
    case GetPasswordReq:
        if(GetPassword(response, request.data1))
        {
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Get Password Failure\n");
            return -1;
        }
        break;
    case DeleteAccountReq:
        if(DeleteAccountService(request.data1))
        {
            strcpy(response, "account deleted\n");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Account Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteBatchReq:
        if(DeleteBatchService(request.data1))
        {
            strcpy(response, "entries deleted\n");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteSSNReq:
        if(DeleteSSNService(request.data1))
        {
            strcpy(response, "Entries Deleted\n");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteCCNReq:
        if(DeleteCCNService(request.data1))
        {
            strcpy(response, "Entries Deleted\n");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeletePasswordReq:
        if(DeletePasswordService(request.data1))
        {
            strcpy(response, "Entries Deleted\n");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case LogoutReq:
        if(Signout(request.data1) == 0)
        {
            strcpy(response, "");
            return 0;
        }
        else
        {
            strcpy(response, "$FAIL$ Signout Failure Somehow\n");
            return -1;
        }
        break;
    default:
        break;
    }
}
