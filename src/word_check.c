#include "word_check.h"
//#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool is_word_equal(const unsigned char *c1, const unsigned char *c2, int length)
{
	for (int i = 0; i < length; i++) {
		if (c1[i] != c2[i]) {
			return false;
		}
	}
	return true;
}

void increment_word(unsigned char *word, int length)
{
	bool overflow = true;
	for (int x = length - 1; x >= 0 && overflow; x--) {
		// byte at pos is at maximum -> set to zero and +1 to word[current-1]
		if (word[x] == 255) {
			word[x] = (unsigned) 0;
		} else {
			word[x] = (int)word[x] + 1;
			overflow = false;
		}
	}
}

void increment_search_space(unsigned char *word, int length)
{
	// if the search space is small we only have 1 'space'
	// search space is in x.x.x...x.1.0.0 to x..1.255.255
	if (length < 3) {
		for (int x = 0; x < length; x++) {
			word[x] = 255;
		}
	} else {
		// special full case
		if (word[0] == 255) {
			word[length-1] = 255;
			word[length-2] = 255;
			return;
		}
		bool overflow = true;
		for (int x = length - 3; x >= 0 && overflow; x--) {
			// byte at pos is at maximum -> set to zero and +1 to word[current-1]
			if (word[x] == 255) {
				word[x] = (unsigned) 0;
			} else {
				word[x] = (int)word[x] + 1;
				overflow = false;
			}
		}
	}
}


bool are_n_bits_equal(unsigned char *word1, unsigned char *word2, int word_length, int nr_bits)
{
	// total shifts to the left
	int total_bits = word_length * 8;
	int shift = total_bits - nr_bits;

	// make a copy
	unsigned char c1[word_length];
	unsigned char c2[word_length];
	for (int i = 0; i < word_length; i++) {
		c1[i] = word1[i];
		c2[i] = word2[i];
	}

	// apply the shift
	for (int x = 0; x < word_length; x++) {
		if (shift >= 8) {
			c1[x] = 0;
			c2[x] = 0;
			shift -= 8;
		} else {
			c1[x] = c1[x] << shift;
			c2[x] = c2[x] << shift;
			break;
		}
	}

	//print_word(c1, word_length);
	//print_word(c2, word_length);

	return is_word_equal(c1,c2,word_length);;
}

void get_LSB(unsigned char* word, unsigned char* lsb, int lsb_length, int word_length, int nr_bits)
{
	//int how_many_chars = nr_bits/8 + 1;
	//unsigned char lsb[how_many_chars];
	// total shifts to the left
	int total_bits = word_length * 8;
	int shift = total_bits - nr_bits;

	// make a copy
	unsigned char c[word_length];
	for (int i = 0; i < word_length; i++) {
		c[i] = word[i];
	}

	// apply the shift
	for (int x = 0; x < word_length; x++) {
		if (shift >= 8) {
			c[x] = 0;
			shift -= 8;
		} else {
			// left then right so delete #shift bits
			c[x] = c[x] << shift;
			c[x] = c[x] >> shift;
			break;
		}
	}

	// copy the bytes in new 
	for (int i = 0; i < lsb_length; i++) {
		lsb[lsb_length-i-1] = c[word_length-i-1];
	}
}

void print_word(unsigned char *c1, int length)
{
	for (int i = 0; i < length; i++) {
		printf("%02x ", c1[i]);
	}
	printf("\n");
}

void print_word(const unsigned char *c1, int length)
{
	for (int i = 0; i < length; i++) {
		printf("%02x ", c1[i]);
	}
	printf("\n");
}
