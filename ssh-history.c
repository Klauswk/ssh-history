#include <unistd.h>
#include <stdio.h>
#include <sodium.h>
#include "crypt.h"

#define GetCurrentDir getcwd
#define SODIUM_STATIC =1

int main(int argc, char **argv) {
     if (sodium_init() < 0) {
        printf("Couldn't find the sodium lib");
        return 1;
    }

 int opterr = 0;
    int listFlag = 0;
    int addFlag = 0;
    int connectFlag = 0;
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
            exit(1);
        default:
            exit(1);
        }

    if (listFlag == 1 && addFlag == 1) {
        printf("Can't list and add at the same time \n");
        exit(1);
    }

    #ifdef _WIN32  
        char buff[FILENAME_MAX];
        GetCurrentDir( buff, FILENAME_MAX );
        strcat(buff,"\\bin\\putty.exe -ssh  root@example.com -pw gg");
        printf("%s",buff);
        system(buff);
    #endif
    #ifdef linux
        char* params[] = {"plink","-ssh","root@example.com", "-pw", "gg",NULL};
        execvp("./bin/plink",params);
    #endif
    
    return 0;
}
