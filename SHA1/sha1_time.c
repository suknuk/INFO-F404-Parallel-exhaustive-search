//#include <stdio.h>
#include <iostream>
#include <time.h>
#include <mpi.h>
#include <openssl/sha.h>

int main()
{

	const unsigned char str[] = "Hello, world!";
	unsigned char hash[SHA_DIGEST_LENGTH];

	clock_t tic = clock();

	for (int i = 0; i < 10000000; i++) {
		SHA1(str, sizeof(str) -1 , hash);
	}

	clock_t toc = clock();
	
//	printf("Elapsed: %f seconds\n", (double(toc - tic) / CLOCKS_PER_SEC));
	std::cout << "Elapsed: " << (double(toc - tic) / CLOCKS_PER_SEC) << " seconds" << std::endl;

/*	printf("%s\n", str);

	for (int i = 0; i < 20; i++)
	{
		printf("%02x", hash[i]);
	}
	printf("\n");
*/
	return 0;
}
