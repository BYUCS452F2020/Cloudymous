#ifndef RESPONSES
#define RESPONSES

enum response_type { RegisterResp, LoginResp, LogoutResp, 
                    PostSSNResp, PostCCNResp, PostPasswordResp, 
                    GetSSNResp, GetCCNResp, GetPasswordResp, 
                    DeleteAccountResp, DeleteBatchResp, DeleteSSNResp, DeleteCCNResp, DeletePasswordResp};

typedef struct
{
    enum response_type responsetype;
    char data1[64];
    char data2[64];
} Response;

#endif