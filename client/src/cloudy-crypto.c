#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "cloudy-crypto.h"

char* hashWrapper(char* data) {
    char* hash = malloc(SHA512_DIGEST_LENGTH);
    SHA512(data, strlen(data), hash);
    return hash;
}

char* commitmentWrapper(char* data) {
    return "replace with real cryptographic commitment";
}
