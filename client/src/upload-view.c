#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "upload-view.h"
#include "upload-presenter.h"

#define MAX_OPTION 16
#define AUTH_SIZE 17
#define MAX_ITEM_SIZE 65

void selectOptionToUpload(char* authtoken) {
    printf("\tOptions to upload: password, ssn, ccn\n");
    printf("Cloudymous> ");
    char itemtype[MAX_OPTION];
    fgets(itemtype, MAX_OPTION, stdin);
    itemtype[strlen(itemtype) - 1] = '\0';

    char* itemToUpload = malloc(MAX_ITEM_SIZE);

    if (strncmp(itemtype, "password", MAX_OPTION) == 0) {
        itemToUpload = getpass("Cloudymous> Password to upload: ");
    }
    else if (strncmp(itemtype, "ssn", MAX_OPTION) == 0) {
        itemToUpload = getpass("Cloudymous> SSN to upload: ");
    }
    else if (strncmp(itemtype, "ccn", MAX_OPTION) == 0) {
        itemToUpload = getpass("Cloudymous> CCN to upload: ");
    }

    char* msg = getPostRespMsg(authtoken, itemtype, itemToUpload);
    if (strcmp("Upload Successful\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
    free(itemToUpload);
}