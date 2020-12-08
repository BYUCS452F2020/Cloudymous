#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "signin-presenter.h"
#include "client-communicator.h"

#define AUTH_SIZE 17

char* getLoginRespMsg(char* uname, char* pswd, char* authtoken) {
    Request req;
    req.requesttype = LoginReq;
    strncpy(req.data1, uname, strlen(uname)+1);
    strncpy(req.data2, pswd, strlen(pswd)+1);
    //Response *resp = malloc(sizeof(Response));
    Response resp;
    // Encrypt password
    // Remember to free the resources for pswd_e
    char* pswd_e = hashWrapper(pswd);
    // Call a communicator
    connect();
    if (sendRequest(req, &resp) < 0) {
        return "Sign-in error\n";
    }
    // Check response
    char *msg = "Sign-in Successful\n\n";
    strncpy(authtoken, resp.data1, AUTH_SIZE);
    free(pswd_e);
    //free(resp);
    return msg;
}
