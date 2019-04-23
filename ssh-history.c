#include <unistd.h>
#include <stdio.h>
#include <sodium.h>
#include "crypt.h"
#include "connection_repository.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#endif
#define GetCurrentDir getcwd
#define SODIUM_STATIC = 1

static void printConnections(Connection **connections, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("\nId: %s  \t User: %s \t Ip: %s",
               connections[i]->id, connections[i]->user, connections[i]->ip);
    }
}

static void setStdinEcho(int enable)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable)
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

static char **splitString(char *string, const char *token, size_t *size)
{

    size_t i = 0;

    size_t numberOfTokens = 0;

    while (string[i] != '\0')
    {
        if (string[i] == *token)
        {
            numberOfTokens++;
        }
        i++;
    }

    char **splitedString = malloc(sizeof(char *) * numberOfTokens);

    *size = numberOfTokens;

    if (splitedString)
    {
        size_t idx = 0;
        char *splitString = strtok(string, token);

        while (splitString)
        {
            *(splitedString + idx++) = strdup(splitString);
            splitString = strtok(0, token);
        }
        *(splitedString + idx) = NULL;
    }

    return splitedString;
}

static void startConnection(char *connection, char *password)
{
    #ifdef _WIN32
        char buff[FILENAME_MAX];
        GetCurrentDir(buff, FILENAME_MAX);
        strcat(buff, "\\bin\\putty.exe -ssh ");
        strcat(buff, connection);
        if(password) {
            strcat(buff,"-pw ");
            strcat(buff,password);
        }
        printf("%s", buff);
        system(buff);
    #endif
    #ifdef linux
        char *params[] = {"plink", "-ssh", argv[optind], NULL};
        execvp("./bin/plink", params);
    #endif
}

int main(int argc, char **argv)
{
    if (sodium_init() < 0)
    {
        printf("Couldn't find the sodium lib");
        return 1;
    }

    int listFlag = 0;
    int addFlag = 0;
    int getAndConnectFlag = 0;
    char *sshConnection = NULL;
    char *id = NULL;
    int c;

    while ((c = getopt(argc, argv, "la:c:")) != -1)
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
            getAndConnectFlag = 1;
            id = optarg;
            break;
        case '?':
            if (optopt == 'a')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            exit(1);
        default:
            exit(1);
        }

    if (listFlag == 1 && addFlag == 1)
    {
        printf("Can't list and add at the same time \n");
        exit(1);
    }

    if (addFlag == 1)
    {
        size_t size;
        char *token = "@";

        char **splitedString = splitString(sshConnection, token, &size);

        Connection *connection = malloc(sizeof(Connection));
        connection->user = strdup(splitedString[0]);
        connection->ip = strdup(splitedString[1]);

        unsigned char *password = malloc(sizeof(char) * 512);

        printf("\nType your password or ENTER to leave blank\n");
        setStdinEcho(0);
        if (fgets(password, sizeof password, stdin) != NULL)
        {
            size_t len = strlen(password);
            if (len > 0 && password[len - 1] == '\n')
            {
                password[--len] = '\0';
            }
        }

        if (strlen(password) > 0)
        {
            encrypt_password(&password);
            connection->password = password;
        }
        else
        {
            connection->password = "";
        }
        setStdinEcho(1);
        addConnection(connection);

        return 0;
    }

    if (listFlag == 1)
    {
        size_t size = 0;
        Connection **connections = getListOfConnections(&size);

        printConnections(connections, size);

        return 0;
    }

    if (getAndConnectFlag == 1)
    {
        Connection *connection = malloc(sizeof(Connection));
        connection->id = id;
        getConnection(connection);

        printf("\nId: %s  \t User: %s \t Ip: %s \t Password: %s",
               connection->id, connection->user, connection->ip, connection->password);
               
        char stringConnection[FILENAME_MAX];
        unsigned char *decryptPassword = NULL;

        if(connection->password) {
            decryptPassword = malloc(sizeof(char) * 512);
            decrypt_password(connection->password, &decryptPassword);

            printf("\n decrypted password: %s", decryptPassword);
        }
        
        return 0;

        strcat(stringConnection, connection->user);
        strcat(stringConnection, "@");
        strcat(stringConnection, connection->ip);

        startConnection(stringConnection, decryptPassword);
    }
    else if (argv[optind])
    {
        startConnection(argv[optind], NULL);
    }

    return 0;
}
