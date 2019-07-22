#include "connection_repository.h"
#include "logger.h"

static int callback(void *, int, char **, char **);

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

static int callback(void *list, int numberOfColumns, char **columnValues,
                    char **columnNames)
{
    Node *node = (Node *)list;

    if (numberOfColumns == 0)
    {
        return 0;
    }

    Connection *connection = malloc(sizeof(Connection));

    connection->id = strdup(columnValues[ID_COLUMNN]);
    connection->user = strdup(columnValues[USER_COLUMNN]);
    connection->ip = strdup(columnValues[IP_COLUMNN]);

    if (columnValues[3] != NULL)
    {
        connection->password = strdup(columnValues[PASSWORD_COLUMNN]);
    }

    insert(node, connection);

    return 0;
}

static int createTable(sqlite3 *db)
{
    int rc;

    char *err_msg = 0;

    char *sql = "CREATE TABLE IF NOT EXISTS Users(Id INTEGER PRIMARY KEY, User TEXT NOT NULL, Ip TEXT NOT NULL, Password BLOB);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        log_error("Failed to create table\n");
        log_error("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    log_debug("Sucessfully load/created database");
}

static size_t listLength(Node *item)
{
    Node *cur = item;
    size_t size = 0;

    if(cur->connection != NULL) {
        ++size;
    }

    while (cur->next != NULL)
    {
        ++size;

        cur = cur->next;
    }

    return size;
}

static int getListConnections(sqlite3 *db, Node *node)
{
    char *sql = "SELECT Id, User, Ip, Password FROM Users";

    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, callback, node, &err_msg);

    if (rc != SQLITE_OK)
    {
        log_error("Failed to select data\n");
        log_error("SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    return 0;
}

static sqlite3 *openAndCreateDb()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    //rc = sqlite3_open(":memory:", &db);

    rc = sqlite3_open(".db", &db);

    if (rc != SQLITE_OK)
    {

        log_error("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    if (createTable(db) == 1)
    {
        return NULL;
    }

    return db;
}

static Connection **fromNodeToList(Node *node, size_t *size)
{
    size_t listSize = listLength(node);

    Connection **connections = malloc(sizeof(Connection *) * listSize);

    for (int i = 0; i < listSize; i++)
    {
        connections[i] = node->connection;

        node = node->next;
    }

    *size = listSize;

    return connections;
}

Connection **getListOfConnections(size_t *size)
{
    sqlite3 *db = openAndCreateDb();

    Node *node = malloc(sizeof(Node));
    node->connection = NULL;
    node->next = NULL;

    getListConnections(db, node);

    sqlite3_close(db);

    return fromNodeToList(node, size);
}

Connection *getConnection(Connection *connection)
{
    sqlite3 *db = openAndCreateDb();

    char *sql = "SELECT Id, User, Ip, Password FROM Users where Id = ?";

    sqlite3_stmt *pStmt;

    char *err_msg = 0;

    int rc = sqlite3_prepare_v3(db, sql, -1, 0, &pStmt, 0);

    if (rc != SQLITE_OK)
        log_error("SQL error on line:%d msg:%s \n", __LINE__, sqlite3_errmsg(db));

    rc = sqlite3_bind_text(pStmt, 1, connection->id, -1, 0);

    do
    {
        rc = sqlite3_step(pStmt);
        switch (rc)
        {
        case SQLITE_DONE:
            break;

        case SQLITE_ROW:
        {
            int size = sqlite3_column_count(pStmt);

            for (int i = 0; i < size; i++)
            {
                const char *value = NULL;

                if(i == PASSWORD_COLUMNN) {
                    value = sqlite3_column_blob(pStmt, i);
                } else {
                    value = sqlite3_column_text(pStmt, i);
                }
                log_debug("Column %d: %s\n", i, value);

                switch (i)
                {
                case ID_COLUMNN:
                    break;
                case USER_COLUMNN:
                    connection->user = strdup(value);
                    break;
                case IP_COLUMNN:
                    connection->ip = strdup(value);
                    break;
                case PASSWORD_COLUMNN:
                    if(value)
                        connection->password = strdup(value);
                    break;
                }
            }
        }
        break;

        default:
            break;
        }
    } while (rc == SQLITE_ROW);

    sqlite3_close(db);

    return 0;
}

Connection *addConnection(Connection *connection)
{
    sqlite3 *db = openAndCreateDb();

    char *sql = "INSERT INTO Users(User,Ip,Password) VALUES(?,?,?);";

    sqlite3_stmt *pStmt;

    int rc = sqlite3_prepare(db, sql, -1, &pStmt, 0);

    char *err_msg = 0;

    if (rc != SQLITE_OK)
    {

        log_error("Cannot prepare statement: %s\n", sqlite3_errmsg(db));

        return NULL;
    }

    rc = sqlite3_bind_text(pStmt, 1, connection->user, -1, 0);
    rc = sqlite3_bind_text(pStmt, 2, connection->ip, -1, 0);
    rc = sqlite3_bind_text(pStmt, 3, connection->password, -1, 0);

    rc = sqlite3_step(pStmt);

    if (rc != SQLITE_DONE)
    {
        log_error("\nExecution failed: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_finalize(pStmt);

    return connection;
}

int removeConnection(Connection *connection)
{
    sqlite3 *db = openAndCreateDb();

    char *sql = "delete from Users where Id = ?";

    sqlite3_stmt *pStmt;

    int rc = sqlite3_prepare(db, sql, -1, &pStmt, 0);

    char *err_msg = 0;

    if (rc != SQLITE_OK)
    {

        log_error("Cannot prepare statement: %s\n", sqlite3_errmsg(db));

        return 1;
    }

    rc = sqlite3_bind_text(pStmt, 1, connection->id, -1, 0);
    
    rc = sqlite3_step(pStmt);

    if (rc != SQLITE_DONE)
    {
        log_error("\nExecution failed: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);

    printf("Connection %s successfully remove.", connection->id);

    return 0;
}
