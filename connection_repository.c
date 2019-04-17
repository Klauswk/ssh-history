#include "connection_repository.h"

int callback(void *, int, char **, char **);

static void insert(Node *root, Connection *con)
{
    Node *link = (Node *)malloc(sizeof(Node));

    link->connection = con;
    link->next = NULL;
    Node *head = root;

    if (root->connection == NULL)
    {
        root->connection = con;
        root->next = NULL;
        return;
    }

    while (root->next != NULL)
    {
        root = root->next;
    }

    root->next = link;

    root = head;
}

int callback(void *list, int argc, char **argv,
             char **azColName)
{
    Node *node = (Node*)list;

    if (argc == 0)
    {
        printf("No connection available\n");
        return 0;
    }

    Connection *connection = malloc(sizeof(Connection));

    connection->user = argv[1];
    connection->password = argv[2];
    connection->ip = argv[3];
    
    insert(node, connection);

    printf("\n%s@%s\n", node->connection->user,node->connection->password);

    return 0;
}

static int createTable(sqlite3 *db)
{
    int rc;

    char *err_msg = 0;

    char *sql = "CREATE TABLE IF NOT EXISTS Users(Id INT, User TEXT, Ip TEXT, Password Text);";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
}

int getAndInsertUser(sqlite3 *db, Connection *connection)
{

    char *sql = "INSERT INTO Users(User,Ip,Password) VALUES(?,?,?);";

    sqlite3_stmt *pStmt;

    int rc = sqlite3_prepare(db, sql, -1, &pStmt, 0);

    char *err_msg = 0;

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));

        return 1;
    }

    rc = sqlite3_bind_text(pStmt, 1, connection->user, -1, 0);
    rc = sqlite3_bind_text(pStmt, 2, connection->ip, -1, 0);
    rc = sqlite3_bind_text(pStmt, 3, connection->password, -1, 0);

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

int getListConnections(sqlite3 *db, Node *node)
{
    char *sql = "SELECT * FROM Users";

    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, callback, node, &err_msg);

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

Connection **getListOfConnections()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    //rc = sqlite3_open(":memory:", &db);

    rc = sqlite3_open(".db", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    if(createTable(db) == 1){
        return NULL;
    }
    
    Node *node = malloc(sizeof(Node));
    node->connection = NULL;
    node->next = NULL;

    getListConnections(db, node);
    
    printf("\n%s@%s\n", node->connection->user,node->connection->password);
    /*Connection *connection = malloc(sizeof(Connection));

    connection->user = "user1";
    connection->password = "blabla";
    connection->ip = "test.com";

    getAndInsertUser(db,connection);*/

    sqlite3_close(db);
}

Connection *getConnection(Connection *connections)
{
    
}

Connection *addConnection(Connection *connection)
{
}

Connection *removeConnection(Connection *connection)
{
}
