#include "Server.h" 

int ParseRequestType(char* input)
{
    const char* typeStirngs[] = {"Register", "Login", "PostSSN", "PostCCN", "PostPassword", "GetSSN", "GetCCN", \
                                "GetPassword", "DeleteAccount", "DeleteBatch", "DeleteSSN", "DeleteCCN", \
                                "DeletePassword", "Logout"};
    for(int i = 0; i < 14; i++)
    {
        if(strcmp(input, typeStirngs[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}



int main(int argc, char* argv[])
{
    if(!strcmp(argv[1], "Drop"))
    {
        DropTables();
        return 0;
    }
    
    Init();
    Request request;
    Response response;
    FILE * authFile;
    char auth[17];
    request.requesttype = ParseRequestType(argv[1]);
    switch (request.requesttype)
    {
        case RegisterReq:
        case LoginReq:
            strcpy(request.data1, argv[2]);
            strcpy(request.data2, argv[3]);
            if(Handle(request, &response) == 0)
            {

                authFile = fopen("auth.txt", "w+");
                fprintf(authFile, "%s", response.data1);
                fclose(authFile);
                
                printf("auth = %s\n", response.data1);
            }
            else
            {
                printf("error\n");
            }
            break;
        case PostSSNReq:
        case PostCCNReq:
        case PostPasswordReq:
            authFile = fopen("auth.txt", "r+"); 
            fgets(auth, 17, authFile);
            fclose(authFile);
            strcpy(request.authtoken, auth);
            strcpy(request.data1, argv[2]);
            if(Handle(request, &response) == 0)
            {
                printf("posted\n");
            }
            else
            {
                printf("error\n");
            }

            break;
        case GetSSNReq:
        case GetCCNReq:
        case GetPasswordReq:
            authFile = fopen("auth.txt", "r+"); 
            fgets(auth, 17, authFile);
            fclose(authFile);
            strcpy(request.authtoken, auth);
            if(Handle(request, &response) == 0)
            {
                printf("Resuts: %s", response.data1);
            }
            else
            {
                printf("error\n");
            }
            
            break;
        case DeleteAccountReq:
        case DeleteBatchReq:
        case DeleteSSNReq:
        case DeleteCCNReq:
        case DeletePasswordReq:
            authFile = fopen("auth.txt", "r+"); 
            fgets(auth, 17, authFile);
            fclose(authFile);
            strcpy(request.authtoken, auth);
            if(Handle(request, &response) == 0)
            {
                printf("Deleted\n");
            }
            else
            {
                printf("error\n");
            }
            
            break;
        case LogoutReq:
            authFile = fopen("auth.txt", "r+"); 
            fgets(auth, 17, authFile);
            fclose(authFile);
            strcpy(request.authtoken, auth);
            if(Handle(request, &response) == 0)
            {
                authFile = fopen("auth.txt", "w");
                fclose(authFile);
                printf("Logged Out\n");
            }
            else
            {
                printf("error\n");
            }
            
            break;
        default:
            printf("unreconized command\n");
            break;
    }
    

        
    

}