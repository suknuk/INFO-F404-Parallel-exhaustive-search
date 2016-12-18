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
	unsigned char start[] = {90 , 255, 254};
	unsigned char end[] = {91, 0, 1};

	std::vector<std::vector<unsigned char*> > a;
	//a = do_compare(t,5,start,end,3);

	//std::cout << str << hash << std::endl;;
	
	unsigned char bit1[] = {90 , 12, 90};
	unsigned char bit2[] = {12 , 13, 90};

	std::cout << "equal bits : " << are_n_bits_equal(bit1,bit2,3,9) << std::endl;


	/*
	for (int i = 0; i < 20; i++)
	{
		printf("%02x", hash[i]);
		
	}
	printf("\n");
	*/
	return 0;
}
