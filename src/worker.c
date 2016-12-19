#include "worker.h"
#include <string.h> //for strlen
#include <stdio.h> //for output
#include <stdlib.h>
#include <unistd.h>
#include <openssl/sha.h> 
#include "word_check.h"

void do_compare(unsigned char* LSB, int LSB_length, int nr_bits, unsigned char* word,
		unsigned char* start_search, unsigned char* end_search, int search_length, int id)
{
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
			// get char array with hex representation of the word
			char word_hex[search_length*2 + 1];
			for (int i = 0; i < search_length; i++) {
				sprintf(&word_hex[i*2], "%02x", word[i]);
			}
			// print findings to file
			char filename[90];
			sprintf(filename, "collisions_%d_bits_on_%d_bytes_msg_%s.csv", 
					nr_bits, search_length, word_hex);
			
			// check if file exists, if not write the headings
			if ( access(filename, W_OK ) == -1) {
				FILE *headings = fopen(filename, "w");
				fprintf(headings, "original word, LSB of hash, found word, matching LSB, matching hash\n");
				fclose(headings);
			}
			
			FILE *file = fopen(filename, "a");

			//print the original word
			for (int i = 0; i < search_length; i ++) {
				fprintf(file, "%02x", word[i]);
			}
			fprintf(file, ",");

			//print original LSB of hash
			for (int i = 0; i < LSB_length; i ++) {
				fprintf(file, "%02x", LSB[i]);
			}
			fprintf(file, ",");

			// print foud word with a matching LSB
			for (int i = 0; i < search_length; i ++) {
				fprintf(file, "%02x", start_search[i]);
			}
			fprintf(file, ",");
			
			// print LSB of the matching word
			for (int i = 0; i < LSB_length; i ++) {
				fprintf(file, "%02x", hash_lsb[i]);
			}
			fprintf(file, ",");
			// the full hash
			for (int i = 0; i < 20; i ++) {
				fprintf(file, "%02x", hash[i]);
			}
			
			fprintf(file, "\n");
			fclose(file);
		}
		increment_word(start_search,search_length);
	}
}


