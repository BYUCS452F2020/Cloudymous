#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "upload-presenter.h"
#include "cloudy-crypto.h"
#include "client-communicator.h"

#define MAX_OPTION 16
#define AUTH_SIZE 17

const char publicKey[]="-----BEGIN PUBLIC KEY-----\n"\
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
"ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
"vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
"fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
"i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
"PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
"wQIDAQAB\n"\
"-----END PUBLIC KEY-----\n";
  
char* getPostRespMsg(char* authtoken, char* itemtype, char* itemToUpload) {
    Request req;
    if (strncmp(itemtype, "password", MAX_OPTION) == 0) {
        req.requesttype = PostPasswordReq;
    }
    else if (strncmp(itemtype, "ssn", MAX_OPTION) == 0) {
        req.requesttype = PostSSNReq;
    }
    else if (strncmp(itemtype, "ccn", MAX_OPTION) == 0) {
        req.requesttype = PostCCNReq;
    }

    strncpy(req.authtoken, authtoken, strlen(authtoken)+1);

    // Encrypt with RSA
    unsigned char encrypted[4098]={};
    int encrypted_length = public_encrypt(itemToUpload,strlen(itemToUpload)+1,publicKey,encrypted);
    strncpy(req.data1, encrypted, encrypted_length);
    Response resp;

    // Call a communicator
    connect();
    if (sendRequest(req, &resp) < 0) {
        return "Upload error\n";
    }

    // Check response
    char *msg = "Upload Successful\n";
    //strncpy(authtoken, resp.data1, AUTH_SIZE);
    return msg;
}
