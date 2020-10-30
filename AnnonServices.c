#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<time.h> 
#include <unistd.h>
  


void AuthMaker(char* auth)
{
    char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$^&*-_=+";
    srand(time(0));
    for(int i = 0; i < 16; i++)
    {
        auth[i] = chars[rand() % 74];
    }
    auth[16] = '\0';
}

int DropTables()
{
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
        fprintf(stderr, "Error Drop tables, %s\n", messageError);
        sqlite3_free(messageError);
    }
    else
    {
        printf("table drop success\n");
    }
    sqlite3_close(DB);
}

int InitTables()
{
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
        fprintf(stderr, "Error Create Table, %s\n", messageError);
        sqlite3_free(messageError);
    }
    else
    {
        printf("table created success\n");
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
        fprintf(stderr, "Error connecting to table\n");
        return 0;
    }
    
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "bad auth bind %s\n", sqlite3_errmsg(DB));
        return 0;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        sqlite3_finalize(res);
        return 1;


    }
    else
        {
            fprintf(stderr, "bad auth\n");
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
        fprintf(stderr, "bad faillog bind %s\n", sqlite3_errmsg(DB));
        return;
    }
    if(sqlite3_step(res) != SQLITE_DONE)
    {
        printf("failed to add fail %s\n", sqlite3_errmsg(DB));
    }
    else
    {
        printf("added fail to user %s\n", username);
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
        fprintf(stderr, "bad faillog bind %s\n", sqlite3_errmsg(DB));
        return;
    }
     if(sqlite3_step(res) != SQLITE_DONE)
    {
        printf("failed to clear fail %s\n", sqlite3_errmsg(DB));
    }
    else
    {
        printf("clear fails of user %s\n", username);
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
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error adding user faile log %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(stderr, "Error adding user faile log %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    else
    {
        printf("added %s faillog\n", username);
    }
    //exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    sqlite3_finalize(res);
    sqlite3_close(DB);
}

void RegisterService(char* auth, char* username, char* password_e)
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
        fprintf(stderr, "Error connecting to table\n");
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
        fprintf(stderr, "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(stderr, "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    else
    {
        printf("added %s\n", username);
    }

    //exit = sqlite3_exec(DB, sql, NULL, 0, &messageError);
    sqlite3_close(DB);
    AddFailTable(username);
}

void LoginService(char* auth, char* username, char* password_e)
{
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT COUNT(*) FROM account WHERE username = ? AND password_e = ?";
    
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
        return;
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, password_e, strlen(password_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
        return;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        
        if(sqlite3_column_int(res, 0) <= 0)
        {
            printf("user %s not found\n", username);
            AddFail(username, DB);
            sqlite3_finalize(res);
            sqlite3_close(DB);
            return;
        }
        else
        {
            
            if(!CheckFailLog(username, DB))
            {
                auth = "too many fails!";
                printf("too many failed logs for user %s", username);
                return;
            }
            ClearFail(username, DB);
            //printf("user %s found\n", username);

        }
        
        
    }
    else
        {
            fprintf(stderr, "user %s not found\n", username);
            auth = "----------------";
            sqlite3_finalize(res);
            sqlite3_close(DB);
            return;
        }
    

    //----------------------------------------------------------------------------------------------------------------------------


    sqlite3_stmt *res2;
    sql = "UPDATE account SET authtoken = ?, timestamp = datetime(datetime('now', '+1 day', 'utc')) WHERE username = ?;";
    AuthMaker(auth);
    exit = sqlite3_prepare_v2(DB, sql, -1, &res2, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res2, 2, username, strlen(username), SQLITE_STATIC);
        sqlite3_bind_text(res2, 1, auth, 16, SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error logging in %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res2) != SQLITE_DONE) {
        fprintf(stderr, "Error adding user %s, %s\n", username,  sqlite3_errmsg(DB));
    }
    else
    {
        //printf("logged in %s\n", username);
    }
    sqlite3_finalize(res2);
    sqlite3_close(DB);
}

void authCheck(char* auth, char* username)
{
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT username FROM account WHERE authtoken = ? ;";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
        return;
    }
    
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, auth, 16, SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "bad auth bind %s\n", sqlite3_errmsg(DB));
        return;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        
        //printf("%s\n", sqlite3_column_text(res, 0));
        char* temp = (char*)sqlite3_column_text(res, 0);
        strcpy(username, temp);
    }
    else
        {
            fprintf(stderr, "bad auth\n");
        }
    
    sqlite3_finalize(res);
    sqlite3_close(DB);
}

char* encrypt(char* username_e, char* username, char* salt)
{
    return username;
}

void PostSSN(char* ssn_e, char* auth)
{
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        printf("Bad authToken\n");
        return;
    }
    printf("%s\n", username);
    
    //printf("username = %s\n", username);
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO ssn(ssn_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        //printf("username = %s\n", username);
        //printf("username len = %d\n", strlen(username));
        sqlite3_bind_text(res, 1, ssn_e, strlen(ssn_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    else
    {
        printf("added ssn\n");
    }
    free(username);
    sqlite3_close(DB);

}

void PostCCN(char* ccn_e, char* auth)
{
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        printf("Bad authToken\n");
        return;
    }
    printf("%s\n", username);
    
    //printf("username = %s\n", username);
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO ccn(ccn_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        //printf("username = %s\n", username);
        //printf("username len = %d\n", strlen(username));
        sqlite3_bind_text(res, 1, ccn_e, strlen(ccn_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    else
    {
        printf("added ssn\n");
    }
    free(username);
    sqlite3_close(DB);

}

void PostPassword(char* password_e, char* auth)
{
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        printf("Bad authToken\n");
        return;
    }
    printf("%s\n", username);
    
    //printf("username = %s\n", username);
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "INSERT INTO password(password_e, username_e) \
                VALUES(?,?)";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        //printf("username = %s\n", username);
        //printf("username len = %d\n", strlen(username));
        sqlite3_bind_text(res, 1, password_e, strlen(password_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
        
    }
    else
    {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    if (sqlite3_step(res) != SQLITE_DONE) {
        fprintf(stderr, "Error adding ssn %s\n",  sqlite3_errmsg(DB));
    }
    else
    {
        printf("added ssn\n");
    }
    free(username);
    sqlite3_close(DB);

}

/*
int GetSSN(char* results, char* auth)
{
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        printf("Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT (select count(DISTINCT ssn_e) from ssn WHERE username_e = ?) as total , \
     ssn_e FROM ssn WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error requesting ssn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    if(step == SQLITE_ROW)
    {
        count = sqlite3_column_int(res, 0);
    }
    else
    {
        return 0;
    }
    FILE * file = fopen("derp.txt", "w");
    //printf("count = %d\n", count);
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(int i = 0; (i < count && i < 1024); ++i)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 1));
        //printf("buffer = %s", buffer);
        strncat(buf2, buffer, strlen(buffer)+1);
        //printf("%s", buf2);
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
*/

int GetSSN(char* results, char* auth)
{
    char* username = (char*)malloc(sizeof(char) * 64);
    authCheck(auth, username);
    
    if(strcmp(username, "") == 0)
    {
        printf("Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT ssn_e FROM ccn WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error requesting ssn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    //FILE * file = fopen("derp.txt", "w");
    //printf("count = %d\n", count);
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        //printf("buffer = %s", buffer);
        strncat(buf2, buffer, strlen(buffer)+1);
        //printf("%s", buf2);
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
        printf("Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT ccn_e FROM ccn WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error requesting ccn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    //FILE * file = fopen("derp.txt", "w");
    //printf("count = %d\n", count);
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        //printf("buffer = %s", buffer);
        strncat(buf2, buffer, strlen(buffer)+1);
        //printf("%s", buf2);
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
        printf("Bad authToken\n");
        return 0;
    }
    sqlite3_stmt *res;
    sqlite3* DB;
    char* sql = "SELECT DISTINCT password_e FROM password WHERE username_e = ?";
    int exit = 0;
    exit = sqlite3_open("test.db", &DB);
    if(exit != SQLITE_OK)
    {
        fprintf(stderr, "Error connecting to table\n");
    }
    exit = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    char* username_e = encrypt(NULL, username, NULL);
    if (exit == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username_e, strlen(username_e), SQLITE_STATIC);
    }
    else
    {
        fprintf(stderr, "Error requesting ssn %s\n",  sqlite3_errmsg(DB));
    }
    int step = sqlite3_step(res);
    int count;
    
    if(step != SQLITE_ROW)
    {
        return 0;
    }
    //FILE * file = fopen("derp.txt", "w");
    //printf("count = %d\n", count);
    char buffer[64];
    char buf2[1024];
    strcpy(buf2, "");
    for(count = 1; (count < 16); count++)
    {
        sprintf(buffer ,"%s\n", sqlite3_column_text(res, 0));
        //printf("buffer = %s", buffer);
        strncat(buf2, buffer, strlen(buffer)+1);
        //printf("%s", buf2);
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

int main(int argc, char** argv) 
{ 
    dropTables();
    initTables();
    char *auth = (char*)malloc(16 * sizeof(char));
    //char *username = (char*)malloc(64 * sizeof(char));
    //username = "---------";
    RegisterService(auth, "test", "pass");
    //printf("%s\n", auth);
    //sleep(1);
    LoginService(auth, "test", "passsss");
    LoginService(auth, "test", "pass");
    //printf("%s\n", auth);W
    //sleep(1);
    //PostCCN("12345", auth);
    //PostCCN("54321", auth);
    //char *results = (char*)malloc(4096);
    //int count = GetCCN(results, auth);
    //printf("%s %i \n", results, count);
    /*
    for(int i = 0; i < count; i++)
    {
        printf("%s  ", results);
    }
    printf("\n");
    */
    return (0); 
    
}
