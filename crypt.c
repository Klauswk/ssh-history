#include "crypt.h"
#include "logger.h"

const static char NOUNCE[crypto_secretbox_KEYBYTES] 
            = {0x7E,0x31,0xE5,0x9B,0x6E,0x14,0x44,0x97,0x95,0x93, 0x4A, 0x14, 0x95, 0x6E, 0xC4, 0x5B, 0xD5, 0xDE, 0x3F, 0x29, 0x19, 0x26, 0x6F, 0x1E};

static void dump_hex_buff(unsigned char buf[], unsigned int len)
{
    log_debug("\n");
    int i;
    for (i = 0; i < len; i++)
        log_debug("%02X ", buf[i]);
    log_debug("\n");
}

static void get_key(unsigned char *key)
{
    if (access(".sk", F_OK) != -1) {
        if ((access(".sk", 4)) != -1) {
            FILE *fp = NULL;

            fp = fopen(".sk", "r");
            
            fgets(key, crypto_secretbox_KEYBYTES, (FILE*)fp);

        } else
            log_debug("Key has been found, but can't be read\n");
    }
    else {
        log_debug("Generating key");

        FILE *fp = NULL;

        fp = fopen(".sk", "w+");

        crypto_secretbox_keygen(key);

        fputs(key, fp);

        fclose(fp);
    }
}

void encrypt_password(unsigned char **password) {
    unsigned char *key = malloc(crypto_secretbox_KEYBYTES * sizeof(unsigned char *));

    get_key(key);
    
    int messageLength = strlen(*password);
    int cipherTextLength = crypto_secretbox_MACBYTES + messageLength;
    char ciphertext[cipherTextLength];
    
    crypto_secretbox_easy(ciphertext, *password, messageLength, NOUNCE, key);

    *password = strdup(ciphertext);

}

void decrypt_password(unsigned char *password, unsigned char **decrypted) {
    unsigned char *key = malloc(crypto_secretbox_KEYBYTES * sizeof(unsigned char *));

    get_key(key);
    
    int messageLength = strlen(password);
     *decrypted = malloc(messageLength * sizeof(char));

    if (crypto_secretbox_open_easy(*decrypted, password, messageLength, NOUNCE, key) != 0) {
        log_debug("\nError");
        exit(-1);
    }
}
