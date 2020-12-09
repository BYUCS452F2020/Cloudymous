#ifndef REQUESTS_H   /* Include guard */
#define REQUESTS_H 
enum request_type {RegisterReq, LoginReq, PostSSNReq, PostCCNReq, PostPasswordReq, GetSSNReq, GetCCNReq,
                        GetPasswordReq, DeleteAccountReq, DeleteBatchReq, DeleteSSNReq, DeleteCCNReq,
                        DeletePasswordReq, LogoutReq};

typedef struct
{
    enum request_type requesttype;
    char authtoken[17]; // 16 chars + \0 = 17 chars
    char data1[64];
    char data2[64];
}Request;
#endif