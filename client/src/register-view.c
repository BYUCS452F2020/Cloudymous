#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "register-view.h"
#include "register-presenter.h"

#define MAX_UNAME 65
#define MAX_PSWD 65

void getNewUsername(char *uname) {
    printf("Cloudymous> Enter new username: ");
    fgets(uname, MAX_UNAME, stdin);
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

void registerCredentials() {
    char uname[MAX_UNAME];
    getNewUsername(uname);
    char *pswd = getNewPassword();
    char *msg = getRegisterRespMsg(uname, pswd);
    if (strcmp("Registration Successful\n\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
}