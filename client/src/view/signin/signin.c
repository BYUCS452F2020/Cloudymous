#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "signin.h"

#define MAX_UNAME 65
#define MAX_PSWD 65

char *getUsername() {
    printf("\
    \tCloudymous> Username: ");
    char uname[MAX_UNAME];
    fgets(uname, MAX_UNAME, stdin);
    return uname;
}

char *getPassword() {
    printf("\
    \tCloudymous> Password: ");
    char pswd[MAX_PSWD];
    fgets(pswd, MAX_PSWD, stdin);
    return pswd;
}

void signin() {
    char *uname = getUsername();
    char *pswd = getPassword();
}