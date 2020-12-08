#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "register-view.h"
#include "register-presenter.h"

#define MAX_UNAME 65
#define MAX_PSWD 65
#define AUTH_SIZE 17

void getNewUsername(char *uname) {
    printf("Cloudymous> Enter new username: ");
    fgets(uname, MAX_UNAME, stdin);
    // Overwrite \n with \0
    uname[strlen(uname) - 1] = '\0';
}

char *getNewPassword() {
    char* pswd = "Replace me";
    char* pswdDup = "replace me";
    while (strncmp(pswd, pswdDup, MAX_PSWD) != 0) {
        printf("\tPassword rules:\n\t1. Must be >= 8 and <= 64 characters.\n\t2. Numbers and punctuation recommended.\n\n");
        pswd = getpass("Cloudymous> Enter new password: ");
        pswdDup = getpass("Cloudymous> Re-enter password: ");
    }
    printf("\n");
    return pswd;
}

void signup(char* authtoken) {
    char uname[MAX_UNAME];
    getNewUsername(uname);
    char* pswd = getNewPassword();
    //char* authtoken = malloc(AUTH_SIZE);
    char* msg = getRegisterRespMsg(uname, pswd, authtoken);
    if (strcmp("Registration Successful\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
}