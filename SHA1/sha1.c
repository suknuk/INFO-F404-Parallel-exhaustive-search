#include <stdio.h>
#include <mpi.h>
#include <openssl/sha.h>

int main()
{
	const unsigned char str[] = "Hello, world!";
	unsigned char hash[SHA_DIGEST_LENGTH];

	SHA1(str, sizeof(str) -1 , hash);
	
	printf("%s\n", str);

	for (int i = 0; i < 20; i++)
	{
		printf("%02x", hash[i]);
	}
	printf("\n");
	return 0;
}
