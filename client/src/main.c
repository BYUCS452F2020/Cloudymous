#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "print-menu.h"
#include "signin-view.h"
#include "register-view.h"
#include "signout-view.h"
#include "delete-view.h"
#include "upload-view.h"
#include "get-view.h"
#include "Server.h"

#define MAXINPUT 11
#define AUTH_SIZE 17
#define SIGNIN_STRLEN 6
#define SIGNUP_STRLEN 6
#define STORE_STRLEN 5
#define GET_STRLEN 3
#define DEL_STRLEN 6
#define UNREG_STRLEN 10
#define SIGNOUT_STRLEN 7
#define EXIT_STRLEN 4
#define HELP_STRLEN 4

void printMenu() {
    printf("\
    \tPlease enter a command from the following menu:\n\
    \tsignin - Authenticates an existing user\n\
    \tsignup - Creates new account\n\
    \tupload - Uploads data to the cloud\n\
    \tget - Retrieves data from the cloud\n\
    \tdelete - Deletes content determined by options\n\
    \tunregister - Deletes all content and account info\n\
    \tsignout - Signs out the user\n\
    \texit - Terminates this client\n\
    \thelp - Prints this list of commands\n\n");
}

int main() {
    Init();
    char* authtoken = malloc(AUTH_SIZE); 
    memset(authtoken, 0, AUTH_SIZE);
    printf("\n\t\tWelcome to Cloudymous\n\n");
    char input[MAXINPUT];
    printMenu();
    printf("Cloudymous> ");
    // More secure to use than gets(); input limit prevents buffer overflow,
    // as long as the input limit is not larger than the buffer size.
    fgets(input, MAXINPUT, stdin);

    while (strncmp("exit", input, EXIT_STRLEN) != 0) {
        if (strncmp("signin", input, SIGNIN_STRLEN) == 0) {
            signin(authtoken);
        }
        else if (strncmp("signup", input, SIGNUP_STRLEN) == 0) {
            signup(authtoken);
        }
        else if (strncmp("upload", input, STORE_STRLEN) == 0) {
            if (strlen(authtoken) > 0) {
                selectOptionToUpload(authtoken);
            }
            else {
                printf("Not currently signed in\n");
            }
        }
        else if (strncmp("get", input, GET_STRLEN) == 0) {
            if (strlen(authtoken) > 0) {
                selectOptionToGet(authtoken);
            }
            else {
                printf("Not currently signed in\n");
            }
        }
        else if (strncmp("delete", input, DEL_STRLEN) == 0) {
            if (strlen(authtoken) > 0) {
                selectOptionToDelete(authtoken);
            }
            else {
                printf("Not currently signed in\n");
            }
        }
        else if (strncmp("unregister", input, UNREG_STRLEN) == 0) {
        }
        else if (strncmp("signout", input, SIGNOUT_STRLEN) == 0) {
            if (strlen(authtoken) > 0) {
                signout(authtoken);
            }
            else {
                printf("Not currently signed in\n");
            }
        }
        else if (strncmp("help", input, HELP_STRLEN) == 0) {
            printMenu();
        }
        printf("Cloudymous> ");
        fgets(input, MAXINPUT, stdin);
    }

    printf("Exiting Cloudymous\n");
    free(authtoken);
    
    return 0;
}