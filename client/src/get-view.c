#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "get-view.h"
#include "get-presenter.h"

#define MAX_OPTION 16
#define AUTH_SIZE 17
#define MAX_ITEM_SIZE 65

void selectOptionToGet(char* authtoken) {
    printf("\tOptions to get: password, ssn, ccn\n");
    printf("Cloudymous> ");
    char itemtype[MAX_OPTION];
    fgets(itemtype, MAX_OPTION, stdin);
    itemtype[strlen(itemtype) - 1] = '\0';

/*     char* itemToGet = malloc(MAX_ITEM_SIZE);

    if (strncmp(itemtype, "password", MAX_OPTION) == 0) {
        itemToGet = getpass("Cloudymous> Password to get: ");
    }
    else if (strncmp(itemtype, "ssn", MAX_OPTION) == 0) {
        itemToGet = getpass("Cloudymous> SSN to get: ");
    }
    else if (strncmp(itemtype, "ccn", MAX_OPTION) == 0) {
        itemToGet = getpass("Cloudymous> CCN to get: ");
    }
 */
    char* msg = getGetRespMsg(authtoken, itemtype);
    if (strcmp("Download Successful\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
    //free(itemToGet);
}