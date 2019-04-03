#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define DATABASE_FILENAME ".db"
#define USER_POSITION 0
#define PASSWORD_POSITION 1
#define HOST_POSITION 2

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
