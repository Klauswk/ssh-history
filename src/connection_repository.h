#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#define ID_COLUMNN 0
#define USER_COLUMNN 1
#define IP_COLUMNN 2
#define PASSWORD_COLUMNN 3

typedef struct
{
    char *id;
    char *user;
    char *ip;
    char *password;
} Connection;

typedef struct node
{
    Connection *connection;
    struct node *next;
} Node;

Connection **getListOfConnections(size_t *size);

Connection *getConnection(Connection* connection);

Connection *addConnection(Connection* connection);

int removeConnection(Connection* connection);
