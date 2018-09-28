#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>

typedef struct
{
    char *user;
    char *ip;
} Connection;

int callback(void *, int, char **, char **);

int getAndInsertUser(char *strConnection, sqlite3 *db, Connection *connection)
{

    char *pch;
    char invert[strlen(strConnection)];

    strcpy(invert, strConnection);

    printf("Splitting string \"%s\" into tokens:\n", invert);

    connection->user = strtok(invert, "@");
    connection->ip = strtok(NULL, "@");

    printf("%s\n", connection->user);
    printf("%s\n", connection->ip);

    char *sql = "INSERT INTO Connection(User,Ip) VALUES(?,?);";

    sqlite3_stmt *pStmt;

    int rc = sqlite3_prepare(db, sql, -1, &pStmt, 0);

    char *err_msg = 0;

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));

        return 1;
    }

    printf("%s , %s\n", connection->user, connection->ip);

    rc = sqlite3_bind_text(pStmt, 1, connection->user, -1, 0);
    rc = sqlite3_bind_text(pStmt, 2, connection->ip, -1, 0);

    rc = sqlite3_step(pStmt);

    if (rc != SQLITE_DONE)
    {

        printf("\nExecution failed: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("\nSucessfull add connection\n");
    }

    sqlite3_finalize(pStmt);

    return 0;
}

int createTable(sqlite3 *db)
{
    int rc;

    char *err_msg = 0;

    char *sql = "CREATE TABLE IF NOT EXISTS Connection(Id INTEGER PRIMARY KEY AUTOINCREMENT, User TEXT, Ip TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
        return 0;
    }
}

int callback(void *NotUsed, int argc, char **argv,
             char **azColName)
{

    NotUsed = 0;

    if (argc == 0)
    {
        printf("No connection available\n");
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {

        printf("%s@%s\n", argv[i], argv[i + 1] ? argv[i + 1] : "NULL");
    }

    printf("\n");

    return 0;
}

int getListConnections(sqlite3 *db)
{
    char *sql = "SELECT * FROM Connection";

    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{

    int opterr = 0;
    int listFlag = 0;
    int addFlag = 0;
    int connectFlag = 0;
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    char *sshConnection = NULL;
    int c;

    while ((c = getopt(argc, argv, "la:e:")) != -1)
        switch (c)
        {
        case 'l':
            listFlag = 1;
            break;
        case 'a':
            addFlag = 1;
            sshConnection = optarg;
            break;
        case 'c':
            connectFlag = 1;
            sshConnection = optarg;
            break;
        case '?':
            if (optopt == 'a' || optopt == 'e')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            return 1;
        default:
            return 1;
        }

    if (listFlag == 1 && addFlag == 1)
    {
        printf("Can't list and add at the same time \n");
        return 1;
    }

    //rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_open("gg", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    if (createTable(db) == 1)
    {
        return 1;
    }

    if (listFlag == 1)
    {
        getListConnections(db);
    }
    //else if(checkSshRegex(sshConnection) == 1) {

    if (addFlag == 1)
    {
        Connection connection;

        getAndInsertUser(sshConnection, db, &connection);
    }

    if (connectFlag == 1)
    {
        execv("ssh");
    }

    sqlite3_close(db);
}