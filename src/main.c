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
	//const unsigned char str[] = "Hello, world!";
	const unsigned char str[] = {90,0,90,90,90};
	unsigned char hash[SHA_DIGEST_LENGTH];

	SHA1(str, sizeof(str) -1 , hash);
	print_word(str, 5);
	print_word(hash,20);

	unsigned char t[] = "hello";
	unsigned char start[] = "abc";	//{90 , 90, 90};
	unsigned char end[] = "abf";	//{90, 90, 95};

	std::vector<std::vector<unsigned char*> > a;
	//a = do_compare(t,5,start,end,3);

	/*
	for (int i = 0; i < 20; i++)
	{
		printf("%02x", hash[i]);
		
	}
	printf("\n");
	*/
	return 0;
}
