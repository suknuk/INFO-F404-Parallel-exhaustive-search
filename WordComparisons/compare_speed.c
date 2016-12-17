#include <iostream>
#include <time.h>
#include <mpi.h>

bool equal(const unsigned char *c1, const unsigned char *c2)
{
	while (*c1 || *c2) {
		if (*c1++ != *c2++) {
			return false;
		}
	}
	return true;
}

int main()
{
	const unsigned char str1[] = "Hello, world!";
	const unsigned char str2[] = "Hello, there!";

	
	clock_t tic = clock();

	for (int i = 0; i < 10000000; i++) {
		bool a = equal(str1, str2);
	}

	clock_t toc = clock();
	
	std::cout << "Elapsed: " << (double(toc - tic) / CLOCKS_PER_SEC) << " seconds" << std::endl;

	return 0;
}
