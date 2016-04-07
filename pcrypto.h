#ifndef __P__CRYPTO_H__
#define __P__CRYPTO_H__

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tomcrypt.h"
#include <assert.h>
#define	P_HASH_SIZE	20
#define P_SING_SIZE 128
#define P_AES_LEN 32


unsigned char *p_ascii_hash(void *in, int in_len);
int p_sha1(unsigned char *shabuf, const unsigned char *in, unsigned long inlen);

int p_sha1_hmac(unsigned char *shabuf, 
		const unsigned char *key, unsigned long keylen,
		const unsigned char *in, unsigned long inlen);
void p_ascii_to_sha1(unsigned char *chars, unsigned char *shabuf); 
unsigned char *p_sha1_to_ascii(unsigned char *shabuf);	// dynamic alloc



#endif
