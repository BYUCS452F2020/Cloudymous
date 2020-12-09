#ifndef UPLOAD_PRESENTER
#define UPLOAD_PRESENTER
#include "cloudy-crypto.h"
#include "client-communicator.h"
#include "../../shared/requests.h"
#include "../../shared/responses.h"

/*
    Does the following:
    1. Prepares a request struct.
    2. Sends struct to server using internet communication code (/net folder).
    3. Checks response and sends appropriate message for view to display.
*/
char* getGetRespMsg(char* authtoken, char* itemtype);

#endif