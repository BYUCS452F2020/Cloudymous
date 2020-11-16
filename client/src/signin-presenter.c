#include <stdlib.h>
#include <string.h>
#include "signin-presenter.h"
#include "../../shared/requests.h"
//#include "AnnonServices.h"
#include "cloudy-crypto.h"

char* signInWithCredentials(char *uname, char *pswd) {
    Request *req = malloc(sizeof(Request));
    req->requesttype = LoginReq;
    strncpy(req->data1, uname, sizeof(uname));
    strncpy(req->data2, pswd, sizeof(pswd));
    Response *resp = malloc(sizeof(Response));
    // Encrypt password
    // Remember to free the resources for pswd_e
    char* pswd_e = hashWrapper(pswd);
    // Call a communicator from the net folder,
    // or if running locally, call LoginService.
    /*
    if (LoginService("replace with real auth", req->data1, pswd_e) == 0) {
        resp->responsetype = LoginResp;
        return 0;
    }
    */
    // Check response
    char *msg = "Sign-in Successful\n\n";
    free(pswd_e);
    free(resp);
    free(req); // Server should free this struct
    return msg;
}
