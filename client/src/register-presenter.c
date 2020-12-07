#include <stdlib.h>
#include <string.h>
#include "register-presenter.h"
#include "client-communicator.h"

char* getRegisterRespMsg(char *uname, char *pswd, char* authtoken) {
    //Request* req = malloc(sizeof(Request));
    Request req;
    req.requesttype = RegisterReq;
    strncpy(req.data1, uname, sizeof(uname));
    strncpy(req.data2, pswd, sizeof(pswd));
    //Response *resp = malloc(sizeof(Response));
    Response resp;
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
    strncpy(authtoken, resp.data1, 64);
    free(pswd_e);
    //free(resp);
    //free(req);
    return msg;
}
