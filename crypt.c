#include <unistd.h>
#include <stdio.h>
#include <sodium.h>
#include <string.h>

#define SODIUM_STATIC = 1

static void get_key(unsigned char *key)
{
    if (access(".sk", F_OK) != -1)
    {
        if ((access(".sk", 4)) != -1)
        {            
            FILE *fp = NULL;

            fp = fopen(".sk", "r");
            
            fgets(key, crypto_secretbox_KEYBYTES, (FILE*)fp);

        }
        else
            printf("Key has been found, but can't be read\n");
    }
    else {
        printf("Generating key");

        FILE *fp = NULL;

        fp = fopen(".sk", "w+");

        crypto_secretbox_keygen(key);

        fputs(key, fp);

        fclose(fp);
    }
}

static void get_password(unsigned char *password, unsigned char key[], unsigned char **decrypted) {
    char nonce[crypto_secretbox_NONCEBYTES];
    int messageLenght = strlen(password);
    int cipherTextLenght = crypto_secretbox_MACBYTES + messageLenght;
    char ciphertext[cipherTextLenght];
    *decrypted = malloc(messageLenght * sizeof(char));

    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(ciphertext, password, messageLenght, nonce, key);

    if (crypto_secretbox_open_easy(*decrypted, ciphertext, cipherTextLenght, nonce, key) != 0) {
        exit(-1);
    }
    
}

void main() {
    unsigned char key[crypto_secretbox_KEYBYTES];
    unsigned char *password = "abc123456";
    unsigned char *decrypted = NULL;

    get_key(&key);
    get_password(password,key,&decrypted);

    printf("Decode: %s", decrypted);
    
}

void dump_hex_buff(unsigned char buf[], unsigned int len)
{
    int i;
    for (i = 0; i < len; i++)
        printf("%02X ", buf[i]);
    printf("\n");
}
