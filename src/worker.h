#ifndef WORKER_H
#define WORKER_H

#include <vector>

std::vector<std::vector<unsigned char*> > do_compare(unsigned char* LSB, 
		unsigned char* start_search, unsigned char* end_search);

#endif
