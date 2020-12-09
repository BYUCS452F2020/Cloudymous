#ifndef CLOUDY_CRYPTO
#define CLOUDY_CRYPTO
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>


char* hashWrapper(char* data);
char* commitmentWrapper(char* data);
RSA * createRSA(const unsigned char * key, int public);
int public_encrypt(unsigned char * data, int data_len, const unsigned char * key, unsigned char *encrypted);
int private_decrypt(unsigned char * enc_data, int data_len, const unsigned char * key, unsigned char *decrypted);
int private_encrypt(unsigned char * data, int data_len, const unsigned char * key, unsigned char *encrypted);
int public_decrypt(unsigned char * enc_data, int data_len, const unsigned char * key, unsigned char *decrypted);

#endif