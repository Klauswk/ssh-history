#include <unistd.h>
#include <stdio.h>
#include <sodium.h>

#define GetCurrentDir getcwd
#define SODIUM_STATIC =1

int main(int argc, char **argv) {
     if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */
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
