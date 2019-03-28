#include <unistd.h>
#include <stdio.h>
#define GetCurrentDir getcwd

int main(int argc, char **argv) {
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
