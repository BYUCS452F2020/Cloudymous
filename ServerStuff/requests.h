#ifndef REQUESTS_H   /* Include guard */
#define REQUESTS_H 
enum request_type {RegisterReq, LoginReq, PostSSNReq, PostCCNReq, PostPasswordReq, GetSSNReq, GetCCNReq,
                        GetPasswordReq, DeleteAccountReq, DeleteBatchReq, DeleteSSNReq, DeleteCCNReq,
                        DeletePasswordReq};

typedef struct
{
    enum request_type requesttype;
    char authtoken[16];
    char data1[64];
    char data2[64];
}Request;
#endif