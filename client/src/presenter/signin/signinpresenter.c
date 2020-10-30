#include <stdlib.h>
#include <string.h>
#include "signinpresenter.h"
#include "requests.h"

char* signInWithCredentials(char *uname, char *pswd) {
    Request *req = malloc(sizeof(Request));
    req->requesttype = LoginReq;
    strncpy(req->data1, uname, sizeof(uname));
    strncpy(req->data2, pswd, sizeof(pswd));
    Response *resp = malloc(sizeof(Response));
    // Call a communicator from the net folder
    // Check response
    char *msg = "\tSign-in Successful\n";
    free(resp);
    free(req); // Server should free this struct
    return msg;
}
