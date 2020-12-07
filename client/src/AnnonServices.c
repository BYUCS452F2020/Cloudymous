#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h>
#include <sqlite3.h>
#include "AnnonServices.h"

static int logOpen = 0;

static FILE * log;

static int randomSeeded = 0;

void setSeed()
{
    if(randomSeeded == 0)
    {
        time_t t;
        srand((unsigned) time(&t));
        randomSeeded = 1;
    }
}

void initLog()
{
    if(logOpen == 0)
    {
        log = fopen("log.txt", "a+");
        logOpen = 1;
    }
}

void CloseLog()
{
    fclose(log);
}

void AuthMaker(char* auth)
{
    char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$^&*-_=+";
    setSeed();
    
    for(int i = 0; i < 16; i++)
    {
        auth[i] = chars[rand() % 73];
    }
    auth[16] = '\0';
}

int DropTables()
{
    initLog();
    sqlite3* DB;
    char* sql = "DROP TABLE IF EXISTS ACCOUNT; \
                DROP TABLE IF EXISTS PASSWORD; \
                DROP TABLE IF EXISTS SSN; \
                DROP TABLE IF EXISTS CCN; \
                DROP TABLE IF EXISTS FAILEDLOG;";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    char* messageError;
    exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error Drop tables, %s\n", messageError);
        sqlite3_free(messageError);
    }
    else
    {
        fprintf(log, "table drop success\n");
    }
    sqlite3_close(DB);
}

int InitTables()
{ 
    initLog();
    sqlite3* DB;
    char* sql = "CREATE TABLE IF NOT EXISTS ACCOUNT( USERNAME TEXT PRIMARY KEY NOT null, PASSWORD_E TEXT NOT null, AUTHTOKEN CHAR(16), TIMESTAMP INTEGER); \
                Create TABLE IF NOT EXISTS PASSWORD(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, PASSWORD_E TEXT, USERNAME_E TEXT NOT NULL); \
                CREATE TABLE IF NOT EXISTS SSN(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, SSN_E TEXT NOT NULL, USERNAME_E TEXT NOT NULL); \
                CREATE TABLE IF NOT EXISTS CCN(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, CCN_E TEXT NOT NULL, USERNAME_E TEXT NOT NULL); \
                CREATE TABLE IF NOT EXISTS FAILEDLOG(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, USERNAME TEXT NOT NULL, TIMESTAMP INTEGER NOT NULL, ATTEMPTCOUNT INTEGER, \
                FOREIGN KEY(USERNAME) REFERENCES ACCOUNT(USERNAME) ON DELETE CASCADE);";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    char* messageError;
    exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    if(exit != SQLITE_OK)
    {
fprintf(log,  "Error Create Table, %s\n", messageError);
        sqlite3_free(messageError);
    }
    else
    {
        fprintf(log, "table created success\n");
    }
    sqlite3_close(DB);
    
}

int CheckFailLog(char* username, sqlite3* DB)
{
    sqlite3_stmt *res;
    char* sql = "SELECT * FROM failedlog WHERE username = ? AND attemptcount < 5 OR datetime('now') > datetime(timestamp);";
    int exit = 0;
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return 0;
    }
    
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "bad auth bind %s\n", sqlite3_errmsg(DB));
        return 0;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        sqlite3_finalize(res);
        return 1;


    }
    else
        {
        fprintf(log,  "bad auth\n");
            return 0;
        }
}

void AddFail(char* username, sqlite3 *DB)
{
    sqlite3_stmt *res;
    char* sql = "UPDATE FAILEDLOG SET ATTEMPTCOUNT = attemptcount + 1, TIMESTAMP = datetime(datetime('now', '+1 day', 'utc')) WHERE username = ?";
    int exit = 0;
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "bad faillog bind %s\n", sqlite3_errmsg(DB));
        return;
    }
    if(sqlite3_step(res) != SQLITE_DONE)
    {
        fprintf(log, "failed to add fail %s\n", sqlite3_errmsg(DB));
    }
    else
    {
        fprintf(log, "added fail to user %s\n", username);
    }
    sqlite3_finalize(res);
}

void ClearFail(char* username, sqlite3 *DB)
{
    sqlite3_stmt *res;
    char* sql = "UPDATE FAILEDLOG SET ATTEMPTCOUNT = 0, TIMESTAMP = datetime(datetime('now', '+1 day', 'utc')) WHERE username = ?";
    int exit = 0;
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "bad faillog bind %s\n", sqlite3_errmsg(DB));
        return;
    }
     if(sqlite3_step(res) != SQLITE_DONE)
    {
        fprintf(log, "failed to clear fail %s\n", sqlite3_errmsg(DB));
    }
    else
    {
        fprintf(log, "clear fails of user %s\n", username);
    }
    sqlite3_finalize(res);
}

void AddFailTable(char* username)
{
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql =  "INSERT INTO failedlog(attemptcount, username, timestamp) \
                VALUES(0, ?, datetime('now', '+1 day', 'utc'))";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error adding user faile log %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error adding user faile log %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    else
    {
        fprintf(log, "added %s faillog\n", username);
    }
    //exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    sqlite3_finalize(res);
    sqlite3_close(DB);
}

int RegisterService(char* auth, char* username, char* password_e, Response* resp)
{

    sqlite3_stmt *res;
    sqlite3* DB;
    AuthMaker(auth);
    char* sql = "INSERT INTO ACCOUNT(username, password_e, authtoken, timestamp) \
                VALUES(?,?,?,datetime('now', '+1 day', 'utc'));";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, password_e, strlen(password_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 3, auth, 16, SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
        return -1;
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
        return -1;
    }
    else
    {
        fprintf(log, "added %s\n", username);
    }

    //exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    sqlite3_close(DB);
    AddFailTable(username);
    return 0;
}

int LoginService(char* auth, char* username, char* password_e, Response* resp)
{
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT COUNT(*) FROM account WHERE username = ? AND password_e = ?";
    
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        sqlite3_close(DB);
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == (int)SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, password_e, strlen(password_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
        sqlite3_close(DB);
        return -1;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        
        if(sqlite3_column_int(res, 0) <= 0)
        {
            fprintf(log, "user %s not found\n", username);
            AddFail(username, DB);
            sqlite3_finalize(res);
            sqlite3_close(DB);
            return -1;
        }
        else
        {
            if(!CheckFailLog(username, DB))
            {
                auth = "too many fails!";
                fprintf(log, "too many failed logs for user %s", username);
                sqlite3_close(DB);
                return -1;
            }
            ClearFail(username, DB);
            /* Get authtoken, if exists */
            int size = sqlite3_column_bytes(res, 3);
            if (size > 0) {
                const char* authtoken = sqlite3_column_text(res, 3);
                strncpy(auth, authtoken, 16);
            }
            sqlite3_finalize(res);
            //printf("user %s found\n", username);
        }
    }
    else
    {
        fprintf(log,  "user %s not found\n", username);
        sqlite3_finalize(res);
        sqlite3_close(DB);
        return -1;
    }
    
    //----------------------------------------------------------------------------------------------------------------------------
    
    sqlite3_stmt *res2;
    sql = "UPDATE account SET authtoken = ?, timestamp = datetime(datetime('now', '+1 day', 'utc')) WHERE username = ?;";
    AuthMaker(auth);
    exit = sqlite3_prepare_v2(DB, sql, -1, &res2, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res2, 1, auth, 16, SQLITE_STATIC);
        sqlite3_bind_text(res2, 2, username, strlen(username), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error logging in %s, %s\n", username,  sqlite3_errmsg(DB));
        sqlite3_finalize(res2);
        sqlite3_close(DB);
        return -1;
    }
    if (sqlite3_step(res2) != SQLITE_DONE) {
        fprintf(log,  "Error loggin in %s, %s\n", username,  sqlite3_errmsg(DB));
        sqlite3_finalize(res2);
        sqlite3_close(DB);
        return -1;
    }
    else
    {
        sqlite3_finalize(res2);
        sqlite3_close(DB);
        return 0;
    }
    
}

int authCheck(char* auth, char* username)
{
    sqlite3_stmt *res;
    sqlite3* DB;
    strcpy(username,"");
    fprintf(log, "attempting to login auth %s\n", auth);
    char* sql = "SELECT username FROM account WHERE authtoken = ?;";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        sqlite3_finalize(res);
        sqlite3_close(DB);
        return -1;
    }
    
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, auth, 16, SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "bad auth bind %s\n", sqlite3_errmsg(DB));
        sqlite3_finalize(res);
        sqlite3_close(DB);
        return -1;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        
        char* temp = (char*)sqlite3_column_text(res, 0);
        strcpy(username, temp);
    }
    else
        {
            fprintf(log,  "bad auth\n");
            sqlite3_finalize(res);
            sqlite3_close(DB);
            return -1;
        }
    
    sqlite3_finalize(res);
    sqlite3_close(DB);
    return 0;
}

char* encrypt(char* username_e, char* username, char* salt)
{
    return username;
}

int PostSSN(char* ssn_e, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
        fprintf(log, "%s\n", username);
    
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO ssn(ssn_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, ssn_e, strlen(ssn_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    else
    {
        fprintf(log, "added ssn\n");
    }
    free(username);
    sqlite3_close(DB);
    return 0;

}

int PostCCN(char* ccn_e, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
    fprintf(log, "%s\n", username);
    
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO ccn(ccn_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, ccn_e, strlen(ccn_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    else
    {
        fprintf(log, "added ssn\n");
    }
    free(username);
    sqlite3_close(DB);
    return 0;

}

int PostPassword(char* password_e, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
        fprintf(log, "%s\n", username);
    
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO password(password_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, password_e, strlen(password_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error adding ssn %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    else
    {
        fprintf(log, "added ssn\n");
    }
    free(username);
    sqlite3_close(DB);
    return 0;

}

int GetSSN(char* results, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT ssn_e FROM ssn WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error requesting ssn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        strncat(buf2, buffer, strlen(buffer)+1);
        step = sqlite3_step(res);
        if(step != SQLITE_ROW)
        {
            break;
        }
        
        
    }
    strcpy(results, buf2);
    free(username);
    sqlite3_close(DB);
    return count;
}

int GetCCN(char* results, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT ccn_e FROM ccn WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error requesting ccn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        strncat(buf2, buffer, strlen(buffer)+1);
        step = sqlite3_step(res);
        if(step != SQLITE_ROW)
        {
            break;
        }
        
        
    }
    strcpy(results, buf2);
    free(username);
    sqlite3_close(DB);
    return count;
}

int GetPassword(char* results, char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT password_e FROM password WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error requesting ssn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        strncat(buf2, buffer, strlen(buffer)+1);
        step = sqlite3_step(res);
        if(step != SQLITE_ROW)
        {
            break;
        }
        
        
    }
    strcpy(results, buf2);
    free(username);
    sqlite3_close(DB);
    return count;
}

int DeleteCCNService(char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "DELETE FROM ccn WHERE  username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    fprintf(log, "deleated CCNS\n");
    sqlite3_finalize(res);
    sqlite3_close(DB);
    return 0;
}

int DeleteSSNService(char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
    fprintf(log, "%s\n", username);
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "DELETE FROM ssn WHERE  username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    fprintf(log, "deleated SSNS\n");
    sqlite3_finalize(res);
    sqlite3_close(DB);
    return 0;
}

int DeletePasswordService(char* auth)
{

    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
    fprintf(log, "%s\n", username);
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "DELETE FROM password WHERE  username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    fprintf(log, "deleated passwords\n");
    sqlite3_finalize(res);
    sqlite3_close(DB);
    return 0;
}

int DeleteBatchService(char* auth)
{
    int ret = 0;
    if(DeletePasswordService(auth) != 0)
    {
        ret = -1;
    }
    if(DeleteCCNService(auth) != 0)
    {
        ret = -1;
    }
    if(DeleteSSNService(auth) != 0)
    {
        ret = -1;
    }
    return ret;
    
    
}

int DeleteAccountService(char* auth)
{

    if(DeleteBatchService(auth) != 0)
    {
        return -1;
    }
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        fprintf(log, "Bad authToken\n");
        return -1;
    }
    fprintf(log, "%s\n", username);

    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "DELETE FROM failedlog WHERE  username = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        return -1;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    fprintf(log, "deleated failedlog\n");
    sqlite3_reset(res);
    sqlite3_finalize(res);
    sql = "DELETE FROM account WHERE  username = ?";
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(log,  "Error deleting account: %s\n",  sqlite3_errmsg(DB));
        return -1;
    }
    sqlite3_finalize(res);
    sqlite3_close(DB);
    fprintf(log, "deleated account\n");
    return 0;
}

int Signout(char* auth)
{

    sqlite3* DB;
    sqlite3_stmt *res;
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(log,  "Error connecting to table\n");
        sqlite3_close(DB);
        return -1;
    }
    char* sql = "UPDATE account SET authtoken = NULL WHERE authtoken = ?";
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, auth, strlen(auth), SQLITE_STATIC);
    }
    else
    {
        fprintf(log,  "bad logout bind %s\n", sqlite3_errmsg(DB));
        return -1;
    }
    if(sqlite3_step(res) != SQLITE_DONE)
    {
        fprintf(log, "failed to logout %s\n", sqlite3_errmsg(DB));
        return -1;
    }
    else
    {
        sqlite3_finalize(res);
        sqlite3_close(DB);
        fprintf(log,"logged out %s\n", auth);
        return 0;
    }
    
    return 0;
}

