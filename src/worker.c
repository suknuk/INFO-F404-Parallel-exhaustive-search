#include "worker.h"
#include <string.h> //for strlen
#include <iostream> //for output
#include <openssl/sha.h> 
#include "word_check.h"

std::vector<std::vector<unsigned char*> > do_compare(unsigned char* LSB, int LSB_length, 
		unsigned char* start_search, unsigned char* end_search, int search_length)
{
	std::vector<std::vector<unsigned char*> > a;

	// loop
	int counter = 0;
	while (!is_word_equal(start_search, end_search, search_length))
	{
		unsigned char hash[SHA_DIGEST_LENGTH];
		unsigned char str[search_length];
		for (int i = 0; i < search_length; i++) {
			str[i] = start_search[i];
		}
		//const unsigned char* t2 = test;
		
		SHA1(str, sizeof(str)-1, hash);
		

		std::cout << counter << ", " << start_search[0] << start_search[1] << start_search[2] << std::endl;
		counter++;
		increment_word(start_search,search_length);
	}

	return a;
}

