#ifndef SIGNINPRESENTER
#define SIGNINPRESENTER
#include "../../shared/responses.h"

/*
    Does the following:
    1. Prepares a request struct.
    2. Sends struct to server using internet communication code (/net folder).
    3. Checks response and sends appropriate message for view to display.
*/
char* signInWithCredentials(char *uname, char *pswd);

#endif