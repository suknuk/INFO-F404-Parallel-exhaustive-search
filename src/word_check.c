#include "word_check.h"
//#include <iostream>
#include <stdio.h>
#include <string.h>

bool is_word_equal(const unsigned char *c1, const unsigned char *c2, int length)
{
	for (int i = 0; i < length; i++) {
		if (c1[i] != c2[i]) {
			return false;
		}
	}
	return true;
	/*
	while (*c1 || *c2) {
		if (*c1++ != *c2++) {
			return false;
		}
	}
	return true;
	*/
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

	print_word(c1, word_length);
	print_word(c2, word_length);

	return is_word_equal(c1,c2,word_length);;
}

void print_word(unsigned char *c1, int length)
{
	for (int i = 0; i < length; i++) {
		printf("%02x", c1[i]);
	}
	printf("\n");
}
