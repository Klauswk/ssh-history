#include <unistd.h>
#include <stdio.h>
#include <sodium.h>
#include <string.h>

void encrypt_password(unsigned char **password);

void decrypt_password(unsigned char *password, unsigned char **decrypted);
