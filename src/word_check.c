#include "word_check.h"
#include <iostream>
#include <string.h>

bool is_word_equal(const unsigned char *c1, const unsigned char *c2)
{
	while (*c1 || *c2) {
		if (*c1++ != *c2++) {
			return false;
		}
	}
	return true;
}

void increment_word(unsigned char *word)
{
	int length = strlen((char*)word);
	// go from the right
	for (int x = length - 1; x >= 0; x--) {
		// increase word[x] by 1 and check for 255 -> overflow to next position
	}
	std::cout << "increment " << length << std::endl;
}

