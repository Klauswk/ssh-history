#include "connection_repository.h"

static Connection *build_connection(char **token)
{
    Connection *con = malloc(sizeof(Connection *));

    if (token)
    {
        con->ip = token[HOST_POSITION];
        con->user = token[USER_POSITION];
        con->password = token[PASSWORD_POSITION];
    }
    else
    {
        con->user = NULL;
        con->password = NULL;
        con->ip = NULL;
    }

    return con;
}

static int listLength(Node *item)
{
    Node *cur = item;
    int size = 0;

    while (cur->next != NULL)
    {
        ++size;
        cur = cur->next;
    }

    return size;
}

static Connection *str_split(char *a_str, const char a_delim)
{
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    //Replace new line by end character
    if (a_str[strlen(a_str) - 1] == '\n')
    {
        a_str[strlen(a_str) - 1] = '\0';
    }

    Connection *result = malloc(sizeof(Connection *));

    char *newString = malloc(sizeof(char) * strlen(a_str));

    strcpy(newString, a_str);

    char **values = malloc(sizeof(char *) * 3);
    if (result)
    {
        size_t idx = 0;

        char *token = strtok(newString, delim);
        while (token)
        {
            values[idx++] = token;
            strdup(token);
            token = strtok(0, delim);
        }

        result = build_connection(values);
    }

    return result;
}

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

Connection **getListOfConnections()
{
    if (access(DATABASE_FILENAME, 0) != -1)
    {
        if ((access(DATABASE_FILENAME, 4)) != -1)
        {
            FILE *fp = NULL;
            size_t len = 255;
            char *line = malloc(sizeof(char) * len);
            Node *node = malloc(sizeof(Node));

            fp = fopen(DATABASE_FILENAME, "r");

            node->connection = NULL;
            node->next = NULL;

            while (fgets(line, len, fp) != NULL)
            {
                insert(node, str_split(line, ';'));
            }

            int listSize = listLength(node);
            
            Connection **connections = malloc(sizeof(Connection *) * listSize);
            
            for(int i =0 ; i < listSize; i++) {
                connections[i] = node->connection;

                node = node->next;
            }

            return connections;
        }
        else
            printf("Database has been found, but can't be read\n");

        return NULL;
    }
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
