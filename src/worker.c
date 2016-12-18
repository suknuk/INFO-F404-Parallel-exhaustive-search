#include "worker.h"
#include <string.h> //for strlen
#include <iostream> //for output
#include "word_check.h"

std::vector<std::vector<unsigned char*> > do_compare(unsigned char* LSB, 
		unsigned char* start_search, unsigned char* end_search)
{
	std::vector<std::vector<unsigned char*> > a;
	int b = strlen((char*)LSB);	
	std::cout << b << " is the length" << std::endl;

	// loop
	int counter = 0;
	while (!is_word_equal(start_search, end_search))
	{
		std::cout << counter << " " << start_search << std::endl;
		counter++;
		start_search++ ;
		//increment_word(start_search);
	}

	increment_word(start_search);

	return a;
}

