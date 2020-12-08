#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "signout-view.h"
#include "signout-presenter.h"

#define MAX_UNAME 65
#define AUTH_SIZE 17

void signout(char* authtoken) {
    char uname[MAX_UNAME];
    printf("Cloudymous> Username: ");
    fgets(uname, MAX_UNAME, stdin);
    uname[strlen(uname) - 1] = '\0';

    char* msg = getLogoutRespMsg(uname, authtoken);
    if (strcmp("Sign-out Successful\n\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
}