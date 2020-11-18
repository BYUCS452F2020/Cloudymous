#include <stdlib.h>
#include <string.h>
#include "signin-presenter.h"
#include "client-communicator.h"

char* signInWithCredentials(char *uname, char *pswd) {
    //Request* req = malloc(sizeof(Request));
    Request req;
    req.requesttype = LoginReq;
    strncpy(req.data1, uname, sizeof(uname));
    strncpy(req.data2, pswd, sizeof(pswd));
    //Response *resp = malloc(sizeof(Response));
    Response resp;
    // Encrypt password
    // Remember to free the resources for pswd_e
    char* pswd_e = hashWrapper(pswd);
    // Call a communicator
    connect();
    sendRequest(req, resp);
    // Check response
    char *msg = "Sign-in Successful\n\n";
    free(pswd_e);
    //free(resp);
    //free(req);
    return msg;
}
