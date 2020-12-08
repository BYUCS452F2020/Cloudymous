#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "register-presenter.h"
#include "client-communicator.h"

#define AUTH_SIZE 17

char* getRegisterRespMsg(char *uname, char *pswd, char* authtoken) {
    //Request* req = malloc(sizeof(Request));
    Request req;
    req.requesttype = RegisterReq;
    strncpy(req.data1, uname, strlen(uname)+1);
    strncpy(req.data2, pswd, strlen(pswd)+1);
    //Response *resp = malloc(sizeof(Response));
    Response resp;
    resp.responsetype = RegisterResp;
    // Encrypt password
    // Remember to free the resources for pswd_e
    char* pswd_e = hashWrapper(pswd);
    // Call a communicator
    connect();
    if (sendRequest(req, resp) < 0) {
        return "Registration error\n";
    }
    // Check response
    char *msg = "Registration Successful\n\n";
    strncpy(authtoken, resp.data1, AUTH_SIZE);
    free(pswd_e);
    //free(resp);
    //free(req);
    return msg;
}
