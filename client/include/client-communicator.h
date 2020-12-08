#ifndef CLIENT_COMMUNICATOR
#define CLIENT_COMMUNICATOR

#include "../../shared/requests.h"
#include "../../shared/responses.h"

void connect();
int sendRequest(Request req, Response* resp);

#endif