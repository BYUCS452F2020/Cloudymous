#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "delete-view.h"
#include "delete-presenter.h"

#define MAX_OPTION 16
#define AUTH_SIZE 17
#define MAX_ITEM_SIZE 65

void selectOptionToDelete(char* authtoken) {
    printf("\tOptions to delete: password, ssn, ccn\n");
    printf("Cloudymous> ");
    char itemtype[MAX_OPTION];
    fgets(itemtype, MAX_OPTION, stdin);
    itemtype[strlen(itemtype) - 1] = '\0';

    char* itemToDelete = malloc(MAX_ITEM_SIZE);

    if (strncmp(itemtype, "password", MAX_OPTION) == 0) {
        itemToDelete = getpass("Cloudymous> Password to delete: ");
    }
    else if (strncmp(itemtype, "ssn", MAX_OPTION) == 0) {
        itemToDelete = getpass("Cloudymous> SSN to delete: ");
    }
    else if (strncmp(itemtype, "ccn", MAX_OPTION) == 0) {
        itemToDelete = getpass("Cloudymous> CCN to delete: ");
    }

    char* msg = getDeleteRespMsg(authtoken, itemtype, itemToDelete);
    if (strcmp("Delete Successful\n", msg) != 0) {
        fprintf(stderr, "%s", msg);
    }
    else {
        printf("%s", msg);
    }
    free(itemToDelete);
}