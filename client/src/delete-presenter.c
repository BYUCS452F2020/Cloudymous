#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "delete-presenter.h"
#include "client-communicator.h"

#define MAX_OPTION 16
#define AUTH_SIZE 17

char* getDeleteRespMsg(char* authtoken, char* itemtype, char* itemToDelete) {
    Request req;
    if (strncmp(itemtype, "password", MAX_OPTION) == 0) {
        req.requesttype = DeletePasswordReq;
    }
    else if (strncmp(itemtype, "ssn", MAX_OPTION) == 0) {
        req.requesttype = DeleteSSNReq;
    }
    else if (strncmp(itemtype, "ccn", MAX_OPTION) == 0) {
        req.requesttype = DeleteCCNReq;
    }

    strncpy(req.authtoken, authtoken, strlen(authtoken)+1);
    strncpy(req.data1, hashWrapper(itemToDelete), strlen(itemToDelete)+1);
    Response resp;

    // Call a communicator
    connect();
    if (sendRequest(req, &resp) < 0) {
        return "Delete error\n";
    }

    // Check response
    char *msg = "Delete Successful\n";
    //strncpy(authtoken, resp.data1, AUTH_SIZE);
    return msg;
}
