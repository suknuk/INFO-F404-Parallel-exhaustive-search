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
	//print_word(str, 5);
	//print_word(hash,20);

	unsigned char t[] = "hello";
	unsigned char start[] = "abc";
	unsigned char end[] = "abf";

	std::vector<std::vector<unsigned char*> > a;
	a = do_compare(t,5,10,start,end,3);
	
	//unsigned char t1[] = {1,0,0,2};
	//std::cout << sizeof(t1) << std::endl;

	return 0;
}
