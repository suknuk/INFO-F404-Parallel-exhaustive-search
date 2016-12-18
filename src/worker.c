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
	while (!is_word_equal(start_search, end_search, search_length))
	{
		// initi
		unsigned char hash1[SHA_DIGEST_LENGTH];
		unsigned char str[search_length+1];
		for (int i = 0; i < search_length; i++) {
			str[i] = start_search[i];
		}
		str[search_length] = 0; // 0 so other functions understand the end
	
		print_word(str,4);
		SHA1(str, sizeof(str)-1, hash1);
		print_word(hash1,20);

		std::cout << std::endl;	
		
		increment_word(start_search,search_length);
	}

	return a;
}


