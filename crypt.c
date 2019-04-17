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
    int messageLength = strlen(password);
    int cipherTextLength = crypto_secretbox_MACBYTES + messageLength;
    char ciphertext[cipherTextLength];
    *decrypted = malloc(messageLength * sizeof(char));

    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(ciphertext, password, messageLength, nonce, key);

    if (crypto_secretbox_open_easy(*decrypted, ciphertext, cipherTextLength, nonce, key) != 0) {
        exit(-1);
    }
    
}

void decrypt_password(unsigned char *password, unsigned char **decrypted) {
    unsigned char *key = malloc(crypto_secretbox_KEYBYTES * sizeof(unsigned char *));

    get_key(key);
    get_password(password,key,decrypted);

    printf("Decode: %s", *decrypted);
    
}

static void dump_hex_buff(unsigned char buf[], unsigned int len)
{
    int i;
    for (i = 0; i < len; i++)
        printf("%02X ", buf[i]);
    printf("\n");
}
