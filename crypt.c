#include "crypt.h"

static void get_key(unsigned char *key)
{
    if (access(".sk", F_OK) != -1) {
        if ((access(".sk", 4)) != -1) {
            FILE *fp = NULL;

            fp = fopen(".sk", "r");
            
            fgets(key, crypto_secretbox_KEYBYTES, (FILE*)fp);

        } else
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
    int messageLength = crypto_secretbox_MACBYTES + 128;
    *decrypted = malloc(messageLength * sizeof(char));

    randombytes_buf(nonce, sizeof nonce);

    if (crypto_secretbox_open_easy(*decrypted, messageLength, strlen(password), nonce, key) != 0) {
        printf("\nError decrypting password \n");
        exit(-1);
    }
    printf("\n\n decrypted : %s",decrypted);    
}

void encrypt_password(unsigned char **password) {
    unsigned char *key = malloc(crypto_secretbox_KEYBYTES * sizeof(unsigned char *));

    get_key(key);
    
    char nonce[crypto_secretbox_NONCEBYTES];
    int messageLength = 128;
    int cipherTextLength = crypto_secretbox_MACBYTES + 128;
    char ciphertext[cipherTextLength];
    
    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(ciphertext, *password, messageLength, nonce, key);

    *password = strdup(ciphertext);
}

void decrypt_password(unsigned char *password, unsigned char **decrypted) {
    unsigned char *key = malloc(crypto_secretbox_KEYBYTES * sizeof(unsigned char *));

    get_key(key);
    get_password(password,key,decrypted);

    printf("\nDecode: %s", *decrypted);
    
}

static void dump_hex_buff(unsigned char buf[], unsigned int len)
{
    int i;
    for (i = 0; i < len; i++)
        printf("%02X ", buf[i]);
    printf("\n");
}
