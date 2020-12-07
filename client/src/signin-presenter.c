#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "signin-presenter.h"
#include "client-communicator.h"

char* getLoginRespMsg(char* uname, char* pswd, char* authtoken) {
    //Request* req = malloc(sizeof(Request));
    Request req;
    req.requesttype = LoginReq;
    strncpy(req.data1, uname, sizeof(uname));
    strncpy(req.data2, pswd, sizeof(pswd));
    //Response *resp = malloc(sizeof(Response));
    Response resp;
    resp.responsetype = LoginResp;
    // Encrypt password
    // Remember to free the resources for pswd_e
    char* pswd_e = hashWrapper(pswd);
    // Call a communicator
    connect();
    if (sendRequest(req, resp) < 0) {
        return "Sign-in error\n";
    }
    // Check response
    char *msg = "Sign-in Successful\n\n";
    strncpy(authtoken, resp.data1, 64);
    free(pswd_e);
    //free(resp);
    //free(req);
    return msg;
}
