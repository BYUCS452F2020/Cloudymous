#include "client-communicator.h"

void connect() {
    /*  Empty. Should be implemented when we can figure
        out how to establish a TLS connection. */
}

void sendRequest(Request req, Response resp) {
    // Get data from server
    // Fill data in response
    int ret = Handle(req, &resp);
}