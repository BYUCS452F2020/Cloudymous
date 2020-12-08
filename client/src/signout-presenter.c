#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "signout-presenter.h"
#include "client-communicator.h"

#define AUTH_SIZE 17

char* getLogoutRespMsg(char* uname, char* authtoken) {
    Request req;
    req.requesttype = LogoutReq;
    strncpy(req.data1, uname, strlen(uname)+1);
    //Response *resp = malloc(sizeof(Response));
    Response resp;
    // Call a communicator
    connect();
    if (sendRequest(req, &resp) < 0) {
        return "Sign-out error\n";
    }
    // Check response
    char *msg = "Sign-out Successful\n";
    strncpy(authtoken, resp.data1, AUTH_SIZE);
    //free(resp);
    return msg;
}
