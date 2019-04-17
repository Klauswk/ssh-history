#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

typedef struct
{
    char *user;
    char *ip;
    char *password;
} Connection;

typedef struct node
{
    Connection *connection;
    struct node *next;
} Node;

Connection **getListOfConnections();

Connection *getConnection();

Connection *addConnection(Connection* connection);

Connection *removeConnection(Connection* connection);
