#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "signin-view.h"
#include "signin-presenter.h"

#define MAX_UNAME 65
#define MAX_PSWD 65
#define AUTH_SIZE 17

void getUsername(char *uname) {
    printf("Cloudymous> Username: ");
    fgets(uname, MAX_UNAME, stdin);
    uname[strlen(uname) - 1] = '\0';
}

char *getPassword() {
    char *pswd = getpass("Cloudymous> Password: ");
    printf("\n");
    return pswd;
}

char* signin() {
    char uname[MAX_UNAME];
    getUsername(uname);
    char* pswd = getPassword();
    char* authtoken = malloc(AUTH_SIZE);
    char* msg = getLoginRespMsg(uname, pswd, authtoken);
    if (strcmp("Sign-in Successful\n\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
    return authtoken;
}