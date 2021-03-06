
#include "AnnonServices.h"
#include "../../shared/requests.h"
#include "../../shared/responses.h"


void Init()
{
    
    InitTables();
}

void Drop()
{
    FILE * log;
    log = fopen("log.txt", "w");
    fclose(log);
    DropTables();
}

int Handle(Request request, Response* response)
{
    switch (request.requesttype)
    {
    case RegisterReq:
        if(RegisterService(response->data1, request.data1, request.data2, response) == 0)
        {
            response->responsetype = RegisterResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Register Failure\n");
            return -1;
        }
        break;
    case LoginReq:
        if(LoginService(response->data1, request.data1, request.data2, response) == 0)
        {
            response->responsetype = LoginResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Login Failure\n");
            return -1;
        }
        break;
    case PostSSNReq:
        if(PostSSN(request.data1, request.authtoken) == 0)
        {
            response->responsetype = PostSSNResp;
            return 0;
        }
            
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Post SSN Failure\n");
        }
        break;
    case PostCCNReq:
        if(PostCCN(request.data1, request.authtoken) == 0)
        {
            response->responsetype = PostCCNResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Post CCN Failure\n");
            return -1;
        }
        
        break;
    case PostPasswordReq:
        if(PostPassword(request.data1, request.authtoken) == 0)
        {
            printf("Password posted\n");
            response->responsetype = PostPasswordResp;
            return 0;
        }
        else
        {
            printf("Password not posted\n");
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Post Password Failure\n");
            return -1;
        }
        break;
    case GetSSNReq:
        if(GetSSN(response->data1, request.authtoken))
        {
            response->responsetype = GetSSNResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Get SSN Failure\n");
            return -1;
        }
        
        break;
    case GetCCNReq:
        if(GetCCN(response->data1, request.authtoken))
        {
            response->responsetype = GetCCNResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Get CCN Failure\n");
            return -1;
        }
        break;
    case GetPasswordReq:
        if(GetPassword(response->data1, request.authtoken))
        {
            response->responsetype = GetPasswordResp;
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Get Password Failure\n");
            return -1;
        }
        break;
    case DeleteAccountReq:
        if(!DeleteAccountService(request.authtoken))
        {
            response->responsetype = DeleteAccountResp;
            strcpy(response->data1, "account deleted\n");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Account Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteBatchReq:
        if(!DeleteBatchService(request.authtoken))
        {
            response->responsetype = DeleteBatchResp;
            //strcpy(response->data1, "entries deleted\n");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            //strcpy(response->data1, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteSSNReq:
        if(!DeleteSSNService(request.authtoken))
        {
            //strcpy(response->data1, "Entries Deleted\n");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            //strcpy(response->data1, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeleteCCNReq:
        if(!DeleteCCNService(request.authtoken))
        {
            response->responsetype = DeleteCCNResp;
            //strcpy(response->data1, "Entries Deleted\n");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            //strcpy(response->data1, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case DeletePasswordReq:
        if(!DeletePasswordService(request.authtoken))
        {
            response->responsetype = DeletePasswordResp;
            //strcpy(response->data1, "Entries Deleted\n");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            //strcpy(response->data1, "$FAIL$ Entries Not Deleted Failure\n");
            return -1;
        }
        break;
    case LogoutReq:
        if(Signout(request.authtoken) == 0)
        {
            response->responsetype = LogoutResp;
            strcpy(response->data1, "");
            return 0;
        }
        else
        {
            response->responsetype = ErrorResp;
            strcpy(response->data1, "$FAIL$ Signout Failure Somehow\n");
            return -1;
        }
        break;
    default:
        break;
    }
}
