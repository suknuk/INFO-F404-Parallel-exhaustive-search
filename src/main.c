//#include <stdio.h>		// CL output
#include <iostream>
#include <vector>
#include <string>
#include <mpi.h>		// MPI methods
#include <openssl/sha.h>	// SHA1()
#include "word_check.h"		// is_equal_word()
#include "worker.h"

int main()
{
	const unsigned char str[] = "Hello, world!";
	unsigned char hash[SHA_DIGEST_LENGTH];

	SHA1(str, sizeof(str) -1 , hash);
	

	unsigned char t[] = "hello";
	unsigned char start[] = "abc";
	unsigned char end[] = "abc";

	std::vector<std::vector<unsigned char*> > a;
	a = do_compare(t,start,end);

	//printf("%s\n", str);
	std::cout << str << hash << std::endl;;

	/*
	for (int i = 0; i < 20; i++)
	{
		printf("%02x", hash[i]);
		
	}
	printf("\n");
	*/
	return 0;
}
