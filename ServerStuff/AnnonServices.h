#ifndef ANNON_H   /* Include guard */
#define ANNON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<time.h> 
#include <unistd.h>

void AuthMaker(char* auth);

int DropTables();

int InitTables();

int CheckFailLog(char* username, sqlite3* DB);

void AddFail(char* username, sqlite3 *DB);

void ClearFail(char* username, sqlite3 *DB);

void AddFailTable(char* username);

int RegisterService(char* auth, char* username, char* password_e);

int LoginService(char* auth, char* username, char* password_e);

void authCheck(char* auth, char* username);

char* encrypt(char* username_e, char* username, char* salt);

int PostSSN(char* ssn_e, char* auth);

void PostCCN(char* ccn_e, char* auth);

int PostPassword(char* password_e, char* auth);

int GetSSN(char* results, char* auth);

int GetCCN(char* results, char* auth);

int GetPassword(char* results, char* auth);

int DeleteAccountService(char* auth);

int DeleteCCNService(char* auth);

int DeleteSSNService(char* auth);

int DeletePasswordService(char* auth);

int DeleteBatchService(char* auth);

int Signout(char* auth);
#endif

