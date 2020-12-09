#include <stdlib.h>
#include <string.h>
/* #include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
 */
#include "cloudy-crypto.h"

char* hashWrapper(char* data) {
    char* hash = malloc(SHA512_DIGEST_LENGTH);
    SHA512(data, strlen(data), hash);
    return hash;
}

char* commitmentWrapper(char* data) {
    return "replace with real cryptographic commitment";
}
/* http://hayageek.com/rsa-encryption-decryption-openssl-c/ */
RSA * createRSA(const unsigned char * key, int public)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA");
    }
 
    return rsa;
}
int public_encrypt(unsigned char * data, int data_len, const unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,RSA_PKCS1_PADDING);
    return result;
}
int private_decrypt(unsigned char * enc_data, int data_len, const unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    int result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}
int private_encrypt(unsigned char * data, int data_len, const unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,0);
    int result = RSA_private_encrypt(data_len,data,encrypted,rsa,RSA_PKCS1_PADDING);
    return result;
}
int public_decrypt(unsigned char * enc_data, int data_len, const unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,1);
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,RSA_PKCS1_PADDING);
    return result;
}