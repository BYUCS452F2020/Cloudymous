#ifndef RESPONSES_H
#define RESPONSES_H

enum response_type { RegisterResp, LoginResp, LogoutResp, 
                    PostSSNResp, PostCCNResp, PostPasswordResp, 
                    GetSSNResp, GetCCNResp, GetPasswordResp, 
                    DeleteAccountResp, DeleteBatchResp, DeleteSSNResp, DeleteCCNResp, DeletePasswordResp,
                    ErrorResp};

typedef struct
{
    enum response_type responsetype;
    char data1[64];
    char data2[64];
} Response;

#endif