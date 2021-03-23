/**
 *
 * Program: openssl_task
 * Author: Benedikt Stonig
 * Date: 06.03.2021
 * Description: This programm checks if the ciphertext really is "Schoene Crypto Welt"
 * git-repo: https://github.com/Essigschurke13/School.git
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

int main(int argc, char **argv)
{
    //AES_KEY enc_key;
    AES_KEY dec_key;

    //Cyphertext
    unsigned char cyphertext[] = {0xAA, 0xE3, 0x65, 0x27, 0x2C, 0x81, 0x07,
                                  0x8A, 0xB6, 0x11, 0x6B, 0x36, 0x18, 0x31,
                                  0xD0, 0xF6, 0xA5, 0xD3, 0xC8, 0x58, 0x7E,
                                  0x94, 0x6B, 0x53, 0x0B, 0x79, 0x57, 0x54,
                                  0x31, 0x07, 0xF1, 0x5E};

    //AES key
    unsigned char aes_key[AES_BLOCK_SIZE];
    memset(aes_key, 'B', AES_BLOCK_SIZE);

    //Init vector
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 'B', AES_BLOCK_SIZE);

    //Buffer
    unsigned char *dec_out;
    dec_out = (unsigned char *)malloc(128);
    memset(dec_out, 0, 128);

    //Decrypt
    AES_set_decrypt_key(aes_key, sizeof(aes_key) * 8, &dec_key);
    AES_cbc_encrypt(cyphertext, dec_out, sizeof(cyphertext), &dec_key, iv, AES_DECRYPT);

    //Result
    printf("%s\n", dec_out);

    free(dec_out);
    return 0;
}