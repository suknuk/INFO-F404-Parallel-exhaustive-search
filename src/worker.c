#include "worker.h"
#include <string.h> //for strlen
#include <iostream> //for output
#include <openssl/sha.h> 
#include "word_check.h"

void do_compare(unsigned char* LSB, int LSB_length, int nr_bits,
		unsigned char* start_search, unsigned char* end_search, int search_length, int id)
{
//	print_word(LSB, LSB_length);
	while (!is_word_equal(start_search, end_search, search_length))
	{
		// initializing
		unsigned char hash[SHA_DIGEST_LENGTH];
		unsigned char str[search_length+1];
		for (int i = 0; i < search_length; i++) {
			str[i] = start_search[i];
		}
		str[search_length] = 0; // 0 so other functions understand the end
	
		SHA1(str, sizeof(str)-1, hash);	

		unsigned char hash_lsb[LSB_length];
		get_LSB(hash, hash_lsb, LSB_length, SHA_DIGEST_LENGTH,  nr_bits);

		if (is_word_equal(LSB, hash_lsb, LSB_length)){
			std::cout << id << " found LSB" << std::endl;
			print_word(str, search_length);
			print_word(hash,20);
			//print_word(hash_lsb, LSB_length);
		}
		increment_word(start_search,search_length);
	}
}


