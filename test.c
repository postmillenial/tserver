#include "pcrypto.h"
#include <stdio.h>
int main() {

  unsigned char *a = p_ascii_hash ("test", 4);
  printf ("the hash is %s\n", a);

  float float_test = 11.2;
  unsigned char *b = p_ascii_hash (&float_test, sizeof (float));
  printf ("the sha1 hash of the float is %s\n", b);

  return 0;
}
