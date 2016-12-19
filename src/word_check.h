#ifndef WORD_CHECK_H
#define WORD_CHECK_H

// Checking if two words are equal or not
bool is_word_equal(const unsigned char *c1, const unsigned char *c2, int length);

// Incrementing a unsigned char
void increment_word(unsigned char *word, int length);

// Increment search space
void increment_search_space(unsigned char *word, int length);

// Check bits LSB are equal
bool are_n_bits_equal(unsigned char *c1, unsigned char *c2, int word_length, int LSB_length, int nr_bits);

void get_LSB(unsigned char* word, unsigned char* lsb, int lsb_length, int word_length, int nr_bits);

void print_word(unsigned char *c1, int length);
void print_word(const unsigned char *c1, int length);

#endif
