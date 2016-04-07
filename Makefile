
TARGETS = tserver testsha1

all: $(TARGETS)

tserver: 
	gcc -o tserver tserver.c sha1.c error_to_string.c crypt_argchk.c pcrypto.c -DLTC_NO_ASM -Wall

testsha1:
	gcc -o testsha1 test.c sha1.c error_to_string.c crypt_argchk.c pcrypto.c -DLTC_NO_ASM -Wall
clean:
	rm -f $(TARGETS) *.o
