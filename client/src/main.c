#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "print-menu.h"
#include "signin-view.h"
#include "Server.h"

#define MAXINPUT 11
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
    \tstore - Uploads data to the cloud\n\
    \tget - Retrieves data from the cloud\n\
    \tdelete - Deletes content determined by options\n\
    \tunregister - Deletes all content and account info\n\
    \tsignout - Signs out the user\n\
    \texit - Terminates this client\n\
    \thelp - Prints this list of commands\n\n");
}

int main() {
    Init();
    printf("\n\t\tWelcome to Cloudymous\n\n");
    char input[MAXINPUT];
    printMenu();
    printf("Cloudymous> ");
    // More secure to use than gets(); input limit prevents buffer overflow,
    // as long as the input limit is not larger than the buffer size.
    fgets(input, MAXINPUT, stdin);
    printf("\n");

    while (strncmp("exit", input, EXIT_STRLEN) != 0) {
        if (strncmp("signin", input, SIGNIN_STRLEN) == 0) {
            signin();
        }
        else if (strncmp("signup", input, SIGNUP_STRLEN) == 0) {
        }
        else if (strncmp("store", input, STORE_STRLEN) == 0) {
        }
        else if (strncmp("get", input, GET_STRLEN) == 0) {
        }
        else if (strncmp("delete", input, DEL_STRLEN) == 0) {
        }
        else if (strncmp("unregister", input, UNREG_STRLEN) == 0) {
        }
        else if (strncmp("signout", input, SIGNOUT_STRLEN) == 0) {
        }
        else if (strncmp("help", input, HELP_STRLEN) == 0) {
            printMenu();
        }
        //memset(input, 0, MAXINPUT);
        printf("Cloudymous> ");
        fgets(input, MAXINPUT, stdin);
    }

    printf("Exiting Cloudymous\n");
    
    return 0;
}