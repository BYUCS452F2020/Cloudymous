#ifndef CLIENT_COMMUNICATOR
#define CLIENT_COMMUNICATOR

#include "../../shared/requests.h"
#include "../../shared/responses.h"
#include "../../ServerStuff/Server.h"

void connect();
void sendRequest(Request req, Response resp);

#endif