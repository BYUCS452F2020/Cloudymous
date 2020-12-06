#include "client-communicator.h"
#include "../../ServerStuff/Server.h"

void connect() {
    /*  Empty. Should be implemented when we can figure
        out how to establish a TLS connection. */
}

int sendRequest(Request req, Response resp) {
    // Get data from server
    // Fill data in response
    int ret = Handle(req, &resp);
    return ret;
}