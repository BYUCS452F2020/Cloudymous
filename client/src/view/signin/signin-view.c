#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "signin-view.h"

#define MAX_UNAME 65
#define MAX_PSWD 65

void getUsername(char *uname) {
    printf("Cloudymous> Username: ");
    fgets(uname, MAX_UNAME, stdin);
}

char *getPassword() {
    char *pswd = getpass("Cloudymous> Password: ");
    printf("\n");
    return pswd;
}

void signin() {
    char uname[MAX_UNAME];
    getUsername(uname);
    char *pswd = getPassword();
}