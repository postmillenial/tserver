#include <stdarg.h>
#include <assert.h>
#include "pcrypto.h"

extern const ltc_math_descriptor gmp_desc;


// returns dynamically allocated string w/ ASCII of hash
unsigned char *p_ascii_hash(void *in, int in_len)
{
  unsigned char	hash[P_HASH_SIZE];
  assert(in && in_len);

  int res = p_sha1(hash, in, in_len);
  assert(res == CRYPT_OK);
  unsigned char *s = p_sha1_to_ascii(hash);
  return s;
}



int p_sha1(unsigned char *shabuf, const unsigned char *in, unsigned long inlen) 
{
  hash_state	sha;
    
  sha1_init(&sha);
  sha1_process(&sha, in, inlen);
  sha1_done(&sha, shabuf);
  return CRYPT_OK;
}


void p_ascii_to_sha1(unsigned char *to, unsigned char *from)
{
  int 		i;
  unsigned char	t[2 * P_HASH_SIZE + 1];

  strcpy((char *)t, (char *)from);

  for (i = P_HASH_SIZE-1; i >= 0; i--) {
    t[(i+1) * 2] = 0;
    to[i] = strtol((char *)(t + i*2), NULL, 16);
  }
}

unsigned char *p_sha1_to_ascii(unsigned char *hash)
{
  int i; 
  unsigned char *s = malloc(1 + 2 * P_HASH_SIZE);
  if (!s)
    assert (1);
  for (i = 0; i < P_HASH_SIZE; i++) {
    sprintf((char *)(s + 2 * i), "%02X", hash[i] & 0xFF);
  }
  s[40] = 0;
  return s;
}


    
